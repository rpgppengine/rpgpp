#ifndef _RPGPP_NPATCHVIEW_H
#define _RPGPP_NPATCHVIEW_H

#include "TGUI/Signal.hpp"
#include "components/nPatchLines.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

class NPatchView : public WorldView {
public:
	typedef std::shared_ptr<NPatchView> Ptr;

	NPatchView();

	tgui::SignalTyped<NPatchInfo *> onInfoChanged = {"InfoChanged"};

	Texture texture;
	NPatchInfo *info;
	int scale = 1;

	NPatchLines lines;

	void drawCanvas() override;
	void drawOverlay() override;

	static NPatchView::Ptr create();

	void setup(NPatchInfo *info, Texture texture, int scale);

	bool leftMousePressed(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
};

#endif