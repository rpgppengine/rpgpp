#ifndef _EDITOR_WIDGETS_ACTORANIMATIONSPANEL_H
#define _EDITOR_WIDGETS_ACTORANIMATIONSPANEL_H

#include <memory>
#include <string>

#include "actor.hpp"
#include "basicTypes.hpp"
#include "commands/actorEditCommand.hpp"
#include "edui/childWindow.hpp"
#include "edui/dropdown.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/signal.hpp"
namespace editor {
struct ActorAnimationsPanel : public edui::ChildWindow {
	edui::SignalT<std::unique_ptr<ActorEditCommand>> onCommand;

	ActorAnimationsPanel();

	void update() override;

	void init(Actor *ptr);

private:
	Actor *actor;

	std::shared_ptr<edui::Dropdown> animationsDrop;
	std::shared_ptr<edui::IconButton> playButton;
	std::shared_ptr<edui::HorizontalContainer> framesList;

	void showAnimationFrames(const std::string &id);
	void pushCommand(ActorCommandType type, IVector newFrame = {});

	void showTileSetDialog();
};
}  // namespace editor

#endif
