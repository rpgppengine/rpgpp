#ifndef _RPGPP_RESOURCEUNPACKER_H
#define _RPGPP_RESOURCEUNPACKER_H

#include <cstdint>
#include <sstream>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "cereal/archives/portable_binary.hpp"
#include "gamedata.hpp"
#include "mio.hpp"
#include "raylib.h"
#include "rres-raylib.h"

class ResourceUnpacker {
public:
	ResourceUnpacker();

	void mapFile(const std::string &filePath, std::error_code &ec);
	void close();

	MinData getMinData();
	rresResourceChunk getChunk(unsigned int rresId);
	std::vector<std::pair<uint32_t, rresResourceChunk>> getChunkTypeId(int typeId);
	Image loadImage(const std::string &title);
	std::string loadText(const std::string &title);
	Wave loadWave(const std::string &title);
	Music loadMusic(const std::string &title);
	Font loadFont(const std::string &title);

	template <typename T>
	T loadT(const std::string &title, bool &valid) {
		auto chunk = getChunk(calcId(title));
		if (chunk.data.raw == nullptr) {
			TraceLog(TraceLogLevel::LOG_ERROR, "loadT: Unfortunately the data is nullptr \n");
			T bin;
			valid = false;
			return std::move(bin);
		}

		T bin;
		std::stringstream stream;
		stream.write(static_cast<char *>(chunk.data.raw), chunk.info.packedSize);
		{
			cereal::PortableBinaryInputArchive ar(stream);
			ar(bin);
		}
		valid = true;
		return bin;
	}

	template <typename T>
	std::vector<std::pair<uint32_t, T>> loadAllOfType(int typeId) {
		std::vector<std::pair<uint32_t, T>> vec;

		auto chunks = getChunkTypeId(typeId);
		for (auto &pair : chunks) {
			auto &chunk = pair.second;
			T bin;
			std::stringstream stream;
			stream.write(static_cast<char *>(chunk.data.raw), chunk.info.packedSize);
			{
				cereal::PortableBinaryInputArchive ar(stream);
				ar(bin);
			}
			vec.push_back(std::make_pair(pair.first, std::move(bin)));
		}

		return vec;
	}

private:
	mio::mmap_source fileMap;
	unsigned int calcId(const std::string &title);
};

#endif
