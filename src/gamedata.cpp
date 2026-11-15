#include "gamedata.hpp"

#include <cereal/archives/portable_binary.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/variant.hpp>
#include <cereal/types/vector.hpp>
#include <fstream>

void serializeDataToFile(std::string fileName, GameData data) {
	{
		std::fstream os{fileName, std::ifstream::binary | std::fstream::trunc | std::fstream::out};
		if (!os.is_open()) {
			printf("Cannot open %s for writing. \n", fileName.c_str());
			return;
		}

		cereal::PortableBinaryOutputArchive outa(os);
		outa(data);
	}
	printf("tried to serialize.. \n");
}

GameData deserializeFile(std::string fileName) {
	GameData data{};
	{
		std::ifstream is;
		is.open(fileName, std::ios::in | std::ios::binary);
		cereal::PortableBinaryInputArchive ina(is);
		ina(data);
	}

	return data;
}
