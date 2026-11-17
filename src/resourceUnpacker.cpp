#include <cstdint>
#include <sstream>
#include <utility>

#include "bins.hpp"
#define RRES_IMPLEMENTATION	 // keep these defines here!
#define RRES_RAYLIB_IMPLEMENTATION
#include <cstring>
#include <system_error>

#include "cereal/archives/portable_binary.hpp"
#include "gamedata.hpp"
#include "mio.hpp"
#include "raylib.h"
#include "resourceUnpacker.hpp"

unsigned int ResourceUnpacker::calcId(const std::string &title) {
	auto id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(title.data()), static_cast<int>(title.size()));
	return id;
}

static rresResourceChunkData LoadResourceChunkData(rresResourceChunkInfo info, void *data) {
	rresResourceChunkData chunkData = {0};

	// CRC32 data validation, verify packed data is not corrupted
	unsigned int crc32 = rresComputeCRC32((const unsigned char *)data, info.packedSize);

	if ((rresGetDataType(info.type) != RRES_DATA_NULL) &&
		(crc32 == info.crc32))	// Make sure chunk contains data and data is not corrupted
	{
		// Check if data chunk is compressed/encrypted to retrieve properties + data
		if ((info.compType == RRES_COMP_NONE) && (info.cipherType == RRES_CIPHER_NONE)) {
			// Data is not compressed/encrypted (info.packedSize = info.baseSize)
			chunkData.propCount = ((unsigned int *)data)[0];

			if (chunkData.propCount > 0) {
				chunkData.props = (unsigned int *)RRES_CALLOC(chunkData.propCount, sizeof(unsigned int));
				for (unsigned int i = 0; i < chunkData.propCount; i++)
					chunkData.props[i] = ((unsigned int *)data)[i + 1];
			}

			int rawSize = info.baseSize - sizeof(int) - (chunkData.propCount * sizeof(int));
			chunkData.raw = RRES_CALLOC(rawSize, 1);
			if (chunkData.raw != NULL)
				memcpy(chunkData.raw, ((unsigned char *)data) + sizeof(int) + (chunkData.propCount * sizeof(int)),
					   rawSize);
		} else {
			// Data is compressed/encrypted
			// Just return the loaded resource packed data from .rres file,
			// it's up to the user to manage decompression/decryption on user library
			chunkData.raw = RRES_CALLOC(info.packedSize, 1);
			if (chunkData.raw != NULL) memcpy(chunkData.raw, (unsigned char *)data, info.packedSize);
		}
	}

	if (crc32 != info.crc32) RRES_LOG("RRES: WARNING: [ID %i] CRC32 does not match, data can be corrupted\n", info.id);

	return chunkData;
}

ResourceUnpacker::ResourceUnpacker() {}

void ResourceUnpacker::mapFile(const std::string &filePath, std::error_code &ec) {
	fileMap = mio::make_mmap_source(filePath, ec);
}

void ResourceUnpacker::close() { fileMap.unmap(); }

rresResourceChunk ResourceUnpacker::getChunk(unsigned int rresId) {
	rresResourceChunk chunk = {0};

	if (!fileMap.is_mapped()) return chunk;

	RRES_LOG("RRES: INFO: Loading resource\n");

	rresFileHeader header = {0};
	memcpy(&header, &fileMap[0], sizeof(rresFileHeader));

	if (((header.id[0] == 'r') && (header.id[1] == 'r') && (header.id[2] == 'e') && (header.id[3] == 's')) &&
		header.version == 100) {
		bool found = false;
		char *ptr = &fileMap[sizeof(rresFileHeader)];

		for (int i = 0; i < header.chunkCount; i++) {
			rresResourceChunkInfo info = {0};
			memcpy(&info, ptr, sizeof(rresResourceChunkInfo));
			ptr += sizeof(rresResourceChunkInfo);

			if (info.id == rresId) {
				found = true;

				RRES_LOG("RRES: INFO: Found requested resource id: 0x%08x\n", info.id);
				RRES_LOG("RRES: %c%c%c%c: Id: 0x%08x | Base size: %i | Packed size: %i\n", info.type[0], info.type[1],
						 info.type[2], info.type[3], info.id, info.baseSize, info.packedSize);

				// NOTE: Only loading first matching id resource chunk found but
				// showing a message if additional chunks are detected
				if (info.nextOffset != 0)
					RRES_LOG("RRES: WARNING: Multiple linked resource chunks available for the provided id");

				// Read and resource chunk from file data
				// NOTE: Read data can be compressed/encrypted, it's up to the user library to manage
				// decompression/decryption
				void *data = RRES_CALLOC(info.packedSize, 1);  // Allocate enough memory to store resource data chunk
				memcpy(data, ptr, info.packedSize);

				chunk.data = LoadResourceChunkData(info, data);
				chunk.info = info;

				RRES_FREE(data);
				break;
			}

			ptr += info.packedSize;
		}
	}

	return chunk;
}

std::vector<std::pair<uint32_t, rresResourceChunk>> ResourceUnpacker::getChunkTypeId(int typeId) {
	std::vector<std::pair<uint32_t, rresResourceChunk>> vec;

	rresResourceChunk chunk = {0};

	if (!fileMap.is_mapped()) return vec;

	RRES_LOG("RRES: INFO: Loading resource\n");

	rresFileHeader header = {0};
	memcpy(&header, &fileMap[0], sizeof(rresFileHeader));

	if (((header.id[0] == 'r') && (header.id[1] == 'r') && (header.id[2] == 'e') && (header.id[3] == 's')) &&
		header.version == 100) {
		bool found = false;
		char *ptr = &fileMap[sizeof(rresFileHeader)];

		for (int i = 0; i < header.chunkCount; i++) {
			rresResourceChunkInfo info = {0};
			memcpy(&info, ptr, sizeof(rresResourceChunkInfo));
			ptr += sizeof(rresResourceChunkInfo);

			if (info.type[0] == 'R' && info.type[1] == 'A' && info.type[2] == 'W' && info.type[3] == 'D') {
				// found = true;

				RRES_LOG("RRES: INFO: Found requested resource id: 0x%08x\n", info.id);
				RRES_LOG("RRES: %c%c%c%c: Id: 0x%08x | Base size: %i | Packed size: %i\n", info.type[0], info.type[1],
						 info.type[2], info.type[3], info.id, info.baseSize, info.packedSize);

				// NOTE: Only loading first matching id resource chunk found but
				// showing a message if additional chunks are detected
				if (info.nextOffset != 0)
					RRES_LOG("RRES: WARNING: Multiple linked resource chunks available for the provided id");

				// Read and resource chunk from file data
				// NOTE: Read data can be compressed/encrypted, it's up to the user library to manage
				// decompression/decryption
				void *data = RRES_CALLOC(info.packedSize, 1);  // Allocate enough memory to store resource data chunk
				memcpy(data, ptr, info.packedSize);

				auto chunkData = LoadResourceChunkData(info, data);

				if (chunkData.props[1] == typeId) {
					found = true;
					chunk.data = chunkData;
					chunk.info = info;

					vec.push_back(std::make_pair(info.id, chunk));
				}

				RRES_FREE(data);
			}

			ptr += info.packedSize;
		}
	}

	return vec;
}

MinData ResourceUnpacker::getMinData() {
	MinData minData;

	auto chunk = getChunk(calcId("@GAMEDATA"));
	if (chunk.data.raw == nullptr) return minData;

	std::stringstream stream;
	stream.write(static_cast<char *>(chunk.data.raw), chunk.info.packedSize);
	{
		cereal::PortableBinaryInputArchive ar(stream);
		ar(minData);
	}
	return minData;
}

Image ResourceUnpacker::loadImage(const std::string &title) {
	auto chunk = getChunk(calcId(title));

	if (chunk.data.raw != nullptr && chunk.data.propCount >= 4) {
		uint32_t width = chunk.data.props[0];
		uint32_t height = chunk.data.props[1];
		uint32_t mipmaps = chunk.data.props[2];
		uint32_t format = chunk.data.props[3];

		const auto *pixels = static_cast<const uint8_t *>(chunk.data.raw);
		std::cout << "IMGE: " << width << "x" << height << " | mipmaps: " << mipmaps << " | format: " << format
				  << " | first pixel RGBA: (" << static_cast<int>(pixels[0]) << ", " << static_cast<int>(pixels[1])
				  << ", " << static_cast<int>(pixels[2]) << ", " << static_cast<int>(pixels[3]) << ")\n";
	}

	Image img = LoadImageFromResource(chunk);
	return img;
}

std::string ResourceUnpacker::loadText(const std::string &title) {
	auto chunk = getChunk(calcId(title));
	std::string text = LoadTextFromResource(chunk);
	return text;
}

Wave ResourceUnpacker::loadWave(const std::string &title) {
	auto chunk = getChunk(calcId(title));
	Wave wave = LoadWaveFromResource(chunk);
	return wave;
}

Music ResourceUnpacker::loadMusic(const std::string &title) {
	auto chunk = getChunk(calcId(title));

	MusicBin bin;
	std::stringstream stream;
	stream.write(static_cast<char *>(chunk.data.raw), chunk.info.packedSize);
	{
		cereal::PortableBinaryInputArchive ar(stream);
		ar(bin);
	}

	Music music = LoadMusicStreamFromMemory(bin.fileExt.c_str(), bin.fileData.data(), bin.fileData.size());
	return music;
}

Font ResourceUnpacker::loadFont(const std::string &title) {
	auto chunk = getChunk(calcId(title));
	if (chunk.data.raw == nullptr) {
		TraceLog(TraceLogLevel::LOG_ERROR, "The font was not found: %s", title.c_str());
		return GetFontDefault();
	}

	FontBin bin;
	std::stringstream stream;
	stream.write(static_cast<char *>(chunk.data.raw), chunk.info.packedSize);
	{
		cereal::PortableBinaryInputArchive ar(stream);
		ar(bin);
	}

	Font font = LoadFontFromMemory(bin.ext.c_str(), bin.data.data(), bin.data.size(), 13, nullptr, 256);
	return font;
}
