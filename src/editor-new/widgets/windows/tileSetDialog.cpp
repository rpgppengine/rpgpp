#include "widgets/windows/tileSetDialog.hpp"

#include <memory>

#include "utils/editorConsts.hpp"
#include "widgets/worldView/tileSetCanvas.hpp"

using namespace editor;

TileSetDialog::TileSetDialog() {
	setSize({0, EDITOR_CHILD_WINDOW_WIDTH * 2}, {0, EDITOR_CHILD_WINDOW_HEIGHT * 2});
	centerLayout();
	setText("");

	translationId = "Actor.Animations.EditFrame";

	canvas = std::make_shared<TileSetCanvas>();
	canvas->setSize({1, 0}, {1, 0});
	add(canvas);

	getOkButton().onClicked.connect([this] {
		auto selectedTile = canvas->getSelectedTile();
		if (selectedTile.valid()) {
			onConfirmed.invoke(selectedTile);
		}
	});
}

void TileSetDialog::init(TileSet &tileSet) {
	canvas->init(tileSet);
	canvas->enableSelection();
}

void TileSetDialog::setSelectedTile(IVector &tile) { canvas->setSelectedTile(tile); }
