#ifndef _EDITOR_WIDGETS_WORLDVIEW_TILESETWORLD_H
#define _EDITOR_WIDGETS_WORLDVIEW_TILESETWORLD_H

#include "edui/canvas.hpp"
#include "raylib.h"
#include "utils/gridDraw.hpp"
namespace editor {
struct TileSetCanvas : public edui::Canvas {
	TileSetCanvas();

	void setTexture(Texture texture);
	GridDraw &getGrid();

	void drawOverlay() override;
	void drawCanvas() override;

private:
	GridDraw grid;
	Texture texture;
};
}  // namespace editor

#endif
