#ifndef _EDITOR_WIDGETS_WORLDVIEW_TILESETWORLD_H
#define _EDITOR_WIDGETS_WORLDVIEW_TILESETWORLD_H

#include "basicTypes.hpp"
#include "edui/canvas.hpp"
#include "raylib.h"
#include "tileset.hpp"
#include "utils/gridDraw.hpp"
namespace editor {
struct TileSetCanvas : public edui::Canvas {
	TileSetCanvas();

	void setTexture(Texture texture);
	GridDraw &getGrid();

	void init(TileSet &tileSet);

	void enableSelection();
	void setSelectedTile(IVector &tile);
	IVector getSelectedTile();

	void update() override;
	void drawOverlay() override;
	void drawCanvas() override;

private:
	GridDraw grid;
	Texture texture;

	bool selectionMode = false;
	IVector selectedTile = {-1, -1};
};
}  // namespace editor

#endif
