#ifndef _EDITOR_WIDGETS_WORLDVIEW_ACTORCANVAS_H
#define _EDITOR_WIDGETS_WORLDVIEW_ACTORCANVAS_H

#include "actor.hpp"
#include "edui/canvas.hpp"
#include "utils/boundingBox.hpp"
#include "utils/gridDraw.hpp"

namespace editor {
struct ActorCanvas : public edui::Canvas {
	edui::SignalT2<Rectangle, Rectangle> onCollisionChanged;

	ActorCanvas();

	void update() override;
	void drawCanvas() override;

	void setActor(Actor *ptr);
	void refresh();

private:
	Actor *actor = nullptr;
	editor::BoundingBox box;
	GridDraw grid;
};
}  // namespace editor

#endif
