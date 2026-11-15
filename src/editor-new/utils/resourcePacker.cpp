#include "utils/resourcePacker.hpp"

#include <sstream>

#include "bins.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "editor.hpp"
#include "packHelper.hpp"
#include "raylib.h"
#include "rres.h"

using namespace editor;

ResourcePacker::ResourcePacker() {}

void ResourcePacker::setBasePath(const std::filesystem::path &basePath) { this->basePath = basePath; }

void ResourcePacker::setMinData(MinData data) { this->minData = data; }

void ResourcePacker::pack(const std::filesystem::path &destFile) {
	auto &typesManager = Editor::getInstance().getResources().getTypes();

	std::ofstream out("game.rres", std::ios::binary);

	rresFileHeader header;
	header.id[0] = 'r';
	header.id[1] = 'r';
	header.id[2] = 'e';
	header.id[3] = 's';
	header.version = 100;
	header.chunkCount = typesManager.totalResourceCount(basePath);
	header.cdOffset = 0;
	header.reserved = 0;

	out.write(reinterpret_cast<const char *>(&header), sizeof(header));

	packMinData(out);

	auto filesList = LoadDirectoryFilesEx(basePath.c_str(), "", true);
	for (int i = 0; i < filesList.count; i++) {
		std::string filePath = filesList.paths[i];
		auto extC = GetFileExtension(filePath.c_str());
		if (extC != nullptr) {
			packResource(out, filePath);
		}
	}

	out.close();

	UnloadDirectoryFiles(filesList);
}

void ResourcePacker::packMinData(std::ofstream &out) {
	const std::string id = "@GAMEDATA";

	std::stringstream stream;
	{
		cereal::PortableBinaryOutputArchive ar(stream);
		ar(minData);
	}
	std::string bytes = stream.str();

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = bytes.size();
	props[1] = 0;
	props[2] = 0;
	props[3] = 0;
	chunkData.props = props;
	chunkData.raw = bytes.data();

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + bytes.size();

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'R';
	chunkInfo.type[1] = 'A';
	chunkInfo.type[2] = 'W';
	chunkInfo.type[3] = 'D';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = totalSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, bytes.size());
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packResource(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string ext = GetFileExtension(filePath.c_str());

	auto &typesManager = Editor::getInstance().getResources().getTypes();
	std::string title;
	bool result = typesManager.getTypeTitleByExtension(ext, &title);

	if (result) {
		if (title == "Image") {
			packImage(out, filePath);
		}
		if (title == "Text") {
			packText(out, filePath);
		}
		if (title == "Font") {
			packFont(out, filePath);
		}
		if (title == "Audio") {
			packAudio(out, filePath);
		}

		if (typesManager.hasFactory(title)) {
			packEngineResource(out, filePath);
		}
	}
}

void ResourcePacker::packEngineResource(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	std::string ext = GetFileExtension(filePath.c_str());
	auto &typesManager = Editor::getInstance().getResources().getTypes();
	std::string title;
	bool result = typesManager.getTypeTitleByExtension(ext, &title);

	ResourceType rType = typesManager.getEnum(title);
	auto &factory = typesManager.getFactory(title);
	auto bytes = factory.saveBin(filePath);

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = bytes.size();
	props[1] = rType;
	props[2] = 0;
	props[3] = 0;
	chunkData.props = props;
	chunkData.raw = bytes.data();

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + bytes.size();

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'R';
	chunkInfo.type[1] = 'A';
	chunkInfo.type[2] = 'W';
	chunkInfo.type[3] = 'D';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = totalSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, bytes.size());
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packImage(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	Image img = LoadImage(filePath.c_str());
	size_t dataSize = GetPixelDataSize(img.width, img.height, img.format);

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = img.width;
	props[1] = img.height;
	props[2] = img.format;
	props[3] = img.mipmaps;
	chunkData.props = props;
	chunkData.raw = img.data;

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + dataSize;
	UnloadImage(img);

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'I';
	chunkInfo.type[1] = 'M';
	chunkInfo.type[2] = 'G';
	chunkInfo.type[3] = 'E';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = chunkInfo.baseSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, dataSize);
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packText(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	std::string text = LoadFileText(filePath.c_str());

	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = text.size();
	props[1] = RRES_TEXT_ENCODING_UTF8;
	props[2] = RRES_CODE_LANG_UNDEFINED;
	props[3] = 0;

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + text.size();
	uint32_t dataSize = text.size();

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'T';
	chunkInfo.type[1] = 'E';
	chunkInfo.type[2] = 'X';
	chunkInfo.type[3] = 'T';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = chunkInfo.baseSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, dataSize);
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packAudio(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string ext = GetFileExtension(filePath.c_str());

	if (ext == ".wav") {
		packWave(out, filePath);
	} else {
		packMusic(out, filePath);
	}
}

void ResourcePacker::packWave(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	Wave wave = LoadWave(filePath.c_str());
	size_t dataSize = wave.frameCount * wave.channels * (wave.sampleSize / 8);

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = wave.frameCount;
	props[1] = wave.sampleRate;
	props[2] = wave.sampleSize;
	props[3] = wave.channels;
	chunkData.props = props;
	chunkData.raw = wave.data;

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + dataSize;

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'W';
	chunkInfo.type[1] = 'A';
	chunkInfo.type[2] = 'V';
	chunkInfo.type[3] = 'E';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = chunkInfo.baseSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, dataSize);
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packMusic(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	MusicBin bin;
	int fileDataSize = 0;
	auto fileData = LoadFileData(filePath.c_str(), &fileDataSize);
	for (int i = 0; i < fileDataSize; i++) {
		bin.fileData.push_back(fileData[i]);
	}
	UnloadFileData(fileData);
	bin.fileExt = GetFileExtension(filePath.c_str());

	std::ostringstream str;
	{
		cereal::PortableBinaryOutputArchive out(str);
		out(bin);
	}
	std::string bytes = str.str();

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = bytes.size();
	props[1] = 0;
	props[2] = 0;
	props[3] = 0;
	chunkData.props = props;
	chunkData.raw = bytes.data();

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + bytes.size();
	uint32_t dataSize = bytes.size();

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'R';
	chunkInfo.type[1] = 'A';
	chunkInfo.type[2] = 'W';
	chunkInfo.type[3] = 'D';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = chunkInfo.baseSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, dataSize);
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}

void ResourcePacker::packFont(std::ofstream &out, const std::filesystem::path &filePath) {
	std::string id = std::filesystem::relative(filePath, basePath);

	FontBin bin;
	auto fileData = LoadFileData(filePath.c_str(), &bin.dataSize);
	for (int i = 0; i < bin.dataSize; i++) {
		bin.data.push_back(fileData[i]);
	}
	UnloadFileData(fileData);
	bin.ext = GetFileExtension(filePath.c_str());
	std::ostringstream str;
	{
		cereal::PortableBinaryOutputArchive out(str);
		out(bin);
	}
	std::string bytes = str.str();

	// data
	rresResourceChunkData chunkData;
	chunkData.propCount = 4;
	uint32_t props[4];
	props[0] = bytes.size();
	props[1] = 0;
	props[2] = 0;
	props[3] = 0;
	chunkData.props = props;
	chunkData.raw = bytes.data();

	uint32_t totalSize = sizeof(uint32_t) + sizeof(props) + bytes.size();
	uint32_t dataSize = bytes.size();

	// info
	rresResourceChunkInfo chunkInfo;
	chunkInfo.type[0] = 'R';
	chunkInfo.type[1] = 'A';
	chunkInfo.type[2] = 'W';
	chunkInfo.type[3] = 'D';

	chunkInfo.id = rresComputeCRC32(reinterpret_cast<const unsigned char *>(id.data()), static_cast<int>(id.size()));

	chunkInfo.compType = 0;
	chunkInfo.cipherType = 0;
	chunkInfo.flags = 0;

	chunkInfo.baseSize = totalSize;
	chunkInfo.packedSize = chunkInfo.baseSize;

	chunkInfo.nextOffset = 0;
	chunkInfo.reserved = 0;

	auto buffer = LoadDataBuffer(chunkData, dataSize);
	chunkInfo.crc32 = rresComputeCRC32(buffer, chunkInfo.packedSize);

	out.write(reinterpret_cast<const char *>(&chunkInfo), sizeof(chunkInfo));
	out.write(reinterpret_cast<const char *>(buffer), chunkInfo.packedSize);

	UnloadDataBuffer(buffer);
}
