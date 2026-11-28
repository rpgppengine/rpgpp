#include "widgets/actorAnimationsPanel.hpp"

#include <memory>
#include <string>

#include "animation.hpp"
#include "basicTypes.hpp"
#include "commands/actorEditCommand.hpp"
#include "editor.hpp"
#include "edui/childWindow.hpp"
#include "edui/colorRect.hpp"
#include "edui/dropdown.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/widget.hpp"
#include "icons.hpp"
#include "services/interface.hpp"
#include "widgets/animationFrameButton.hpp"
#include "widgets/windows/newAnimationWindow.hpp"
#include "widgets/windows/tileSetDialog.hpp"

using namespace editor;

ActorAnimationsPanel::ActorAnimationsPanel() {
	translationId = "Actor.Animations";
	hideCloseButton();
	draggable = false;

	topLayout = std::make_shared<edui::HorizontalContainer>();
	topLayout->render->border = 0;
	topLayout->render->as<edui::HorizontalContainerRender>().space = INTERFACE_SPACING;
	topLayout->setSize({1, 0}, {0, edui::EDUI_SECONDARY_HEIGHT});
	add(topLayout);

	animationsDrop = std::make_shared<edui::Dropdown>();
	animationsDrop->setSize({0, 160}, {1, 0});
	topLayout->add(animationsDrop);

	animationsDrop->onValueChanged.connect([this](edui::DropdownValue, edui::DropdownValue newValue) {
		actor->changeAnimation(newValue.val);
		showAnimationFrames(actor->getCurrentAnimationId());
	});

	initPlaybackButtons();
	addVerticalLine();
	initFrameButtons();
	addVerticalLine();
	initAnimationButtons();

	framesList = std::make_shared<edui::HorizontalContainer>();
	framesList->setPosition({0, 0}, {0, (edui::EDUI_SECONDARY_HEIGHT + INTERFACE_SPACING)});
	framesList->setSize({1, 0}, {0, 64});
	add(framesList);
}

void ActorAnimationsPanel::addVerticalLine() {
	auto line = std::make_shared<edui::ColorRect>();
	line->setSize({0, 1}, {1, 0});
	topLayout->add(line);
}

void ActorAnimationsPanel::initPlaybackButtons() {
	playButton = std::make_shared<edui::IconButton>();
	playButton->setTooltip("Play", "Actor.Animations.Play");
	playButton->iconId = ICON_PLAYER_PLAY;
	playButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(playButton);

	playButton->onClicked.connect([this] {
		actor->setIsPlaying(!actor->getIsPlaying());
		playButton->iconId = actor->getIsPlaying() ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY;
	});

	auto stopButton = std::make_shared<edui::IconButton>();
	stopButton->setTooltip("Stop", "Actor.Animations.Stop");
	stopButton->iconId = ICON_PLAYER_STOP;
	stopButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(stopButton);

	stopButton->onClicked.connect([this] {
		actor->setIsPlaying(false);
		actor->setCurrentFrame(0);
		playButton->iconId = ICON_PLAYER_PLAY;
	});
}

void ActorAnimationsPanel::initFrameButtons() {
	auto newFrameButton = std::make_shared<edui::IconButton>();
	newFrameButton->setTooltip("New Frame", "Actor.Animations.NewFrame");
	newFrameButton->iconId = ICON_FILE_ADD;
	newFrameButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(newFrameButton);

	newFrameButton->onClicked.connect([this] { pushCommand(ACTOR_CMD_NEWFRAME); });

	auto removeFrameButton = std::make_shared<edui::IconButton>();
	removeFrameButton->setTooltip("Remove Frame", "Actor.Animations.RemoveFrame");
	removeFrameButton->iconId = ICON_FILE_DELETE;
	removeFrameButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(removeFrameButton);

	removeFrameButton->onClicked.connect([this] { pushCommand(ACTOR_CMD_REMOVEFRAME); });

	auto editFrameButton = std::make_shared<edui::IconButton>();
	editFrameButton->setTooltip("Edit Frame", "Actor.Animations.EditFrame");
	editFrameButton->iconId = ICON_FILE_OPEN;
	editFrameButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(editFrameButton);

	editFrameButton->onClicked.connect([this] { showTileSetDialog(); });
}

void ActorAnimationsPanel::initAnimationButtons() {
	auto newAnimationButton = std::make_shared<edui::IconButton>();
	newAnimationButton->setTooltip("New Animation", "Actor.Animations.NewAnimation");
	newAnimationButton->iconId = ICON_FILETYPE_PLAY_ADD;
	newAnimationButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(newAnimationButton);

	newAnimationButton->onClicked.connect([this] { showNewAnimationDialog(); });

	auto removeAnimationButton = std::make_shared<edui::IconButton>();
	removeAnimationButton->setTooltip("Remove Animation", "Actor.Animations.RemoveAnimation");
	removeAnimationButton->iconId = ICON_FILETYPE_PLAY_DELETE;
	removeAnimationButton->setSize({0, edui::EDUI_SECONDARY_HEIGHT}, {1, 0});
	topLayout->add(removeAnimationButton);

	removeAnimationButton->onClicked.connect([this] { pushCommand(ACTOR_CMD_REMOVEANIM); });
}

void ActorAnimationsPanel::update() {
	edui::ChildWindow::update();

	if (actor != nullptr) {
		for (auto &widget : framesList->widgets) {
			auto &frameButton = widget->as<AnimationFrameButton>();
			if (frameButton.referId == actor->getCurrentFrame()) {
				frameButton.setActiveBorder(true);
			} else {
				frameButton.setActiveBorder(false);
			}
		}
	}
}

void ActorAnimationsPanel::init(Actor *ptr) {
	actor = ptr;

	animationsDrop->clear();
	for (auto &pair : actor->getAnimationsRaw()) {
		animationsDrop->addItem(pair.first);
	}

	animationsDrop->setValue({0, actor->getCurrentAnimationId()});
	showAnimationFrames(actor->getCurrentAnimationId());
}

void ActorAnimationsPanel::showAnimationFrames(const std::string &id) {
	framesList->deleteChildren();

	auto texture = actor->getTileSet().getTexture();
	auto tileSize = actor->getTileSet().getTileSize();
	float ratio = tileSize.y / tileSize.x;

	Animation anim = actor->getAnimationRaw(id);

	int i = 0;
	for (auto &frame : anim.getFrames()) {
		auto button = std::make_shared<AnimationFrameButton>();
		button->referId = i;
		button->setSize({0, static_cast<int>(64 * (ratio))}, {1, 0});
		button->atlasTile = actor->getTileSet().getTile(frame.toFloat());
		button->atlasTileSize = tileSize;

		framesList->add(button);

		button->onClicked.connect([this, i] { actor->setCurrentFrame(i); });

		i++;
	}
}

void ActorAnimationsPanel::pushCommand(ActorCommandType type, IVector newFrame, const std::string &newAnimTitle) {
	auto cmd = std::make_unique<ActorEditCommand>();
	cmd->actor = actor;
	cmd->animationId = actor->getCurrentAnimationId();
	cmd->frameIndex = actor->getCurrentFrame();
	cmd->newFrame = newFrame;
	cmd->newAnimId = newAnimTitle;
	cmd->commandType = type;

	onCommand.invoke(std::move(cmd));
}

void ActorAnimationsPanel::showTileSetDialog() {
	auto window = std::make_shared<TileSetDialog>();
	window->init(actor->getTileSet());
	IVector frame = actor->getCurrentAnimationAtlas();
	window->setSelectedTile(frame);
	window->onConfirmed.connect([this](IVector tile) { pushCommand(ACTOR_CMD_EDITFRAME, tile); });

	Editor::getInstance().getInterface().showTopWindow(window);
}

void ActorAnimationsPanel::showNewAnimationDialog() {
	auto window = std::make_shared<NewAnimationWindow>();
	window->onConfirmed.connect([this](std::string title) { pushCommand(ACTOR_CMD_NEWANIM, {}, title); });

	Editor::getInstance().getInterface().showTopWindow(window);
}
