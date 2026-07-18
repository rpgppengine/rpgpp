#ifndef RPGPP_ACTORVIEW_H
#define RPGPP_ACTORVIEW_H

#include <memory>

#include "TGUI/Signal.hpp"
#include "actor.hpp"
#include "components/resizableCanvasBox.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

constexpr float DEFAULT_ANIMATION_SPEED{2.0f};

class ActorFileView;
class ActorView : public WorldView {
private:
	std::unique_ptr<ResizableCanvasBox> collisionBox;
	std::unique_ptr<ResizableCanvasBox> atlasBox;
	float animationCurrentDuration{0.0f};

	ActorFileView *actorFileView;

public:
	ActorView(ActorFileView *actorFileView);

	Actor *actor{nullptr};
	tgui::Signal onAttributeChanged = {"OnFrameChanged"};

	// Animation Settings
	bool isPlaying{false};
	bool editData{false};
	float animationFrameDuration{DEFAULT_ANIMATION_SPEED};

	typedef std::shared_ptr<ActorView> Ptr;

	void setActor(Actor *actor);
	void setAtlasRect(const Rectangle &rect);
	void setCollisionRect(const Rectangle &rect);

	void drawCanvas() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;

	static ActorView::Ptr create(ActorFileView *actorFileView);
	static ActorView::Ptr create(ActorFileView *actorFileView, Actor *actor);
};

#endif
