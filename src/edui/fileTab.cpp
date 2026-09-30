#include "edui/fileTab.hpp"

#include <memory>

#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/tab.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

FileTab::FileTab() : Tab() {
	render = std::make_unique<FileTabRender>();
	render->padding = 2;
	auto &rend = render->as<FileTabRender>();
	rend.vertAlign = VerticalAlignment::TEXT_CENTER;

	closeButtonPtr = std::make_shared<IconButton>();
	closeButtonPtr->render->border = 0;
	closeButtonPtr->iconId = ICON_CROSS;
	closeButtonPtr->setPosition({1, -iconRectSize}, {0, 0});
	closeButtonPtr->setSize({0, iconRectSize}, {1, 0});

	closeButtonPtr->onMouseEntered.connect([this] {
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
	});

	closeButtonPtr->onMouseLeft.connect([this] {
		if (mouseIsInRect()) {
			render->currentBgColor = render->focusBgColor;
			render->currentBorderColor = render->focusBorderColor;
		}
	});

	closeButtonPtr->onClicked.connect([this] { markDelete(); });
}

void FileTab::applyTheme(const mINI::INIStructure &struc) {
	IconTextButton::applyTheme(struc);
	closeButtonPtr->applyTheme(struc);
	closeButtonPtr->unfocused();
}

void FileTab::processWidget(std::shared_ptr<Widget> &widget) {
	if (widget->mouseIsInRect()) {
		Gui::instance->notifyChild(&widget);
	} else {
		if (widget->notifiedMouseEnter) {
			widget->notifiedMouseEnter = false;
			widget->mouseLeft();
		}
	}
}

void FileTab::update() {
	IconTextButton::update();

	closeButtonPtr->update();
	closeButtonPtr->calcRect(rect);

	if (closeEnabled) {
		std::shared_ptr<Widget> widgetPtr = closeButtonPtr;
		processWidget(widgetPtr);
	}
}

void FileTab::draw() {
	IconTextButton::draw();
	closeButtonPtr->draw();

	auto &rend = render->as<FileTabRender>();

	DrawRectangleLinesEx(rect, 1.0f, rend.currentBorderColor);
}

void FileTab::resizeToFit() {
	auto &rend = render->as<LabelRender>();

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;

	Vector2 textSize = MeasureTextEx(Gui::instance->font, text.c_str(), totalFontSize, spacing);

	float totalWidth = textSize.x + (rend.padding * 2);

	if (iconId > 0) {
		totalWidth += iconRectSize + (rend.padding * 2);
	}

	totalWidth += (iconRectSize) + rend.padding;

	layout.width.offset = totalWidth;
	layout.width.scale = 0;
}

void FileTab::mouseEntered() {
	onMouseEntered.invoke();
	if (!closeButtonPtr->mouseIsInRect()) {
		render->currentBgColor = render->focusBgColor;
		render->currentBorderColor = render->focusBorderColor;
	}
}

void FileTab::mouseLeft() {
	onMouseLeft.invoke();
	render->currentBgColor = render->bgColor;
	render->currentBorderColor = render->borderColor;
}

void FileTab::setActive() {
	Tab::setActive();

	auto &rend = render->as<FileTabRender>();
	closeButtonPtr->render->bgColor = rend.activeColor;
	closeButtonPtr->render->focusBgColor = rend.activeFocusColor;
	closeButtonPtr->unfocused();
}

void FileTab::setInactive() {
	Tab::setInactive();

	auto &rend = render->as<FileTabRender>();
	closeButtonPtr->render->bgColor = rend.inactiveColor;
	closeButtonPtr->render->focusBgColor = rend.inactiveFocusColor;
	closeButtonPtr->unfocused();
}
