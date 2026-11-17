#include <fstream>
#include <sstream>

#include "basicTypes.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "cereal/cereal.hpp"
#include "cereal/types/common.hpp"
#include "raylib.h"
#define STR_IMPLEMENTATION
#include "editor.hpp"

int main() {
	editor::Editor editor;
	editor.run();

	return 0;
}
