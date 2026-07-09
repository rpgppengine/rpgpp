#include "widgets/frameEditor.hpp"

#include <cstddef>
#include <memory>

#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/ToggleButton.hpp"
#include "actor.hpp"
#include "bindTranslation.hpp"
#include "components/frameButton.hpp"
#include "editor.hpp"
#include "fileViews/actorFileView.hpp"
#include "raylib.h"
#include "services/translationService.hpp"

constexpr int MAX_TOP_BAR_HEIGHT = 30;
constexpr int MAX_ANI_BAR_HEIGHT = 50;

FrameEditor::FrameEditor(ActorFileView *fileView, const char *typeName, bool initRenderer)
	: tgui::ScrollablePanel(typeName, initRenderer), fileView(fileView), actorView(fileView->getActorView()) {}

void FrameEditor::onFrameChange(int currentFrame) {}

void FrameEditor::updateFrameButtons(bool reImport) {
	for (auto &button : this->frameButtons) {
		button->updateSprite(reImport);
	}
}

void FrameEditor::init() {
	const auto &actor = actorView->actor;
	TranslationService &ts = Editor::instance->getTranslations();
	auto topBarLayout = tgui::GrowHorizontalLayout::create();
	topBarLayout->setSize({"100%", MAX_TOP_BAR_HEIGHT});
	topBarLayout->getRenderer()->setSpaceBetweenWidgets(10.0f);

	this->directionChooser = tgui::ComboBox::create();

	bindTranslationWithCallback<tgui::ComboBox>(directionChooser, [](tgui::ComboBox::Ptr box, TranslationService &ts) {
		box->removeAllItems();
		for (int i = 0; i <= RPGPP_MAX_DIRECTION; i++)
			box->addItem(ts.getKey(TextFormat("screen.project.actorview.dir%d", i)));
		box->setSelectedItemByIndex(0);
	});

	directionChooser->onItemSelect.connect([this, &actor](const size_t &index) {
		this->changeFrameState(index);

		for (auto btn : frameButtons) {
			frameLayout->remove(btn);
		}
		frameButtons.clear();
		for (int i = 0; i < actor->getAnimationCount(); i++) this->addFrameButton(i);
	});

	topBarLayout->add(directionChooser);

	playPauseButton = tgui::ToggleButton::create();

	bindTranslationWithCallback<tgui::ToggleButton>(
		playPauseButton, [this](tgui::ToggleButton::Ptr button, TranslationService &ts) {
			if (button->isDown()) {
				button->setText(ts.getKey("screen.project.actorview.pause"));
			} else {
				button->setText(ts.getKey("screen.project.actorview.play"));
			}
			button->onToggle.disconnectAll();
			button->onToggle.connect([&, button](const bool &checked) {
				button->setText(checked ? ts.getKey("screen.project.actorview.pause")
										: ts.getKey("screen.project.actorview.play"));
				actorView->isPlaying = checked;
			});
		});
	playPauseButton->setSize({100, "100%"});
	topBarLayout->add(playPauseButton);

	auto editingAtlasData = tgui::ToggleButton::create();
	bindTranslation<tgui::ToggleButton>(editingAtlasData, "screen.project.actorview.edit_anim_data",
										&tgui::ToggleButton::setText);
	editingAtlasData->setSize({200, "100%"});
	editingAtlasData->onToggle.connect([this](const bool isChecked) { actorView->editData = isChecked; });
	topBarLayout->add(editingAtlasData);

	auto delFrame = tgui::Button::create();
	bindTranslation<tgui::Button>(delFrame, "screen.project.actorview.delete", &tgui::Button::setText);
	delFrame->onPress.connect([&] {
		if (actor->getAnimationCount() > 1) {
			// Remove the last frame button.
			actor->removeAnimationFrame(actor->getAnimationDirection(), actor->getCurrentFrame());

			const auto &lastButton = this->frameButtons.back();

			this->frameLayout->remove(lastButton);
			frameButtons.pop_back();

			this->updateFrameButtons();
			this->onAttributeChanged.emit(this);
		}
	});
	topBarLayout->add(delFrame);

	this->add(topBarLayout);

	this->frameLayout = tgui::GrowHorizontalLayout::create();
	frameLayout->getRenderer()->setSpaceBetweenWidgets(3.0f);

	for (int i = 0; i < actor->getAnimationCount(); i++) this->addFrameButton(i);

	auto encaserLayout = tgui::GrowHorizontalLayout::create();
	encaserLayout->add(frameLayout);

	auto addFrame = tgui::Button::create("+");
	addFrame->onPress.connect([&, this] {
		actor->addAnimationFrame(actor->getAnimationDirection(), {0, 0});
		this->addFrameButton(actor->getAnimationCount() - 1);
		this->onAttributeChanged.emit(this);
	});

	encaserLayout->setSize({"100%", MAX_ANI_BAR_HEIGHT});
	encaserLayout->setPosition({0, MAX_TOP_BAR_HEIGHT + 10});
	encaserLayout->add(addFrame);

	this->add(encaserLayout);
}

void FrameEditor::addFrameButton(int index) {
	const auto &actor = actorView->actor;
	auto frameButton = FrameButton::create(index, actor);
	frameButton->onFrameChange.connect([&](const int &index) { actor->setAnimationFrame(index); });

	this->frameButtons.push_back(frameButton);
	this->frameLayout->add(frameButton);
}

FrameEditor::Ptr FrameEditor::create(ActorFileView *fileView) { return std::make_shared<FrameEditor>(fileView); }

tgui::Widget::Ptr FrameEditor::clone() const { return std::make_shared<FrameEditor>(*this); }

void FrameEditor::changeFrameState(int index) {
	const auto &actor = actorView->actor;

	actor->changeAnimation(static_cast<Direction>(index));
	actor->resetAnimation();
}
