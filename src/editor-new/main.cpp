#include <fstream>
#include <sstream>

#include "basicTypes.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/common.hpp"
#include "raylib.h"
#include "serialize/iniArchive.hpp"
#define STR_IMPLEMENTATION
#include "editor.hpp"

struct Struc {
	int a = 10;
	int b = 20;
	NPatchInfo call;

	template <class Archive>
	void serialize(Archive &ar) {
		ar(CEREAL_NVP(a), CEREAL_NVP(b), CEREAL_NVP(call));
	}
};

int main() {
	Struc test;
	std::ofstream stream("out.json");
	IVector vec = {10, 11};

	{
		cereal::JSONOutputArchive ar(stream);
		// vec.serialize(ar);
		ar(CEREAL_NVP(test));
	}

	editor::Editor editor;
	editor.run();

	return 0;
}
