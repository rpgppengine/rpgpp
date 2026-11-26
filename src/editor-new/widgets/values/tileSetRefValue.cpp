#include "widgets/values/tileSetRefValue.hpp"

#include "basicTypes.hpp"
#include "editor.hpp"
#include "icons.hpp"

using namespace editor;

TileSetRefValue::TileSetRefValue() {
	auto projectBase = Editor::getInstance().getResources().getProject().getBasePath();

	input = std::make_shared<edui::FileInput>();
	input->setSize({1, 0}, {1, 0});

	input->lockBasePath(projectBase);
	input->addCategory("TileSet", {".rtiles"}, ICON_BOX_GRID_BIG);

	input->onValueChanged.connect([this, projectBase](edui::FileOption a, edui::FileOption b) {
		std::filesystem::path oldPath = a.fullPath;
		std::filesystem::path newPath = b.fullPath;
		if (oldPath.is_absolute()) {
			oldPath = std::filesystem::relative(a.fullPath);
		}
		if (newPath.is_absolute()) {
			newPath = std::filesystem::relative(b.fullPath);
		}

		TileSetRef old = {oldPath};
		TileSetRef newval = {newPath};

		onValueChanged.invoke(old, newval);
	});

	add(input);
}

TileSetRef TileSetRefValue::getValue() { return input->getValue().fullPath; }

void TileSetRefValue::setValue(const TileSetRef &value) { input->setValue({value.path}); }
