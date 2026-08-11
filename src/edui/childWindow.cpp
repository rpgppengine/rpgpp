#include "edui/childWindow.hpp"

#include <memory>

#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/iconButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

ChildWindow::ChildWindow() : Container() {
	render = std::make_unique<ChildWindowRender>();
	auto closeButton = std::make_shared<IconButton>();
	closeButton->iconId = ICON_CROSS;
	closeButton->setPosition({1, static_cast<int>(-EDUI_CHILDWINDOW_BARHEIGHT)}, {0, 0});
	closeButton->setSize({0, static_cast<int>(EDUI_CHILDWINDOW_BARHEIGHT)},
						 {0, static_cast<int>(EDUI_CHILDWINDOW_BARHEIGHT)});

	std::weak_ptr<IconButton> weakClose = closeButton;
	closeButton->clicked = [this, weakClose] {
		if (auto sharedClose = weakClose.lock()) {
			sharedClose->markDelete();
			markDelete();
		}
	};
	closeButtonPtr = closeButton;
}

Rectangle ChildWindow::getBarRect() { return {rect.x, rect.y, rect.width, EDUI_CHILDWINDOW_BARHEIGHT}; }

void ChildWindow::setTitle(const std::string &title) {
	auto &rend = render->as<ChildWindowRender>();

	this->title = title;
	auto barRect = getBarRect();
	auto padding = paddingRect(barRect, rend.titlePadding);

	if (rend.font == nullptr) return;

	float totalFontSize = rend.font->baseSize * rend.fontSize;

	drawOverflownText(padding, rend.font, rend.fontSize, rend.spacing, title, &shownText);
}

void ChildWindow::update() {
	if (dragging) {
		auto delta = GetMouseDelta();
		layout.x.offset += delta.x;
		layout.y.offset += delta.y;
	}

	Container::update();

	closeButtonPtr->update();

	auto barRect = getBarRect();
	closeButtonPtr->calcRect(barRect);

	if (closeButtonPtr->mouseIsInRect()) {
		std::shared_ptr<Widget> widgetPtr = closeButtonPtr;
		Gui::instance->notifyChild(&widgetPtr);
	} else {
		if (closeButtonPtr->notifiedMouseEnter) {
			closeButtonPtr->notifiedMouseEnter = false;
			closeButtonPtr->mouseLeft();
		}
	}
}

void ChildWindow::draw() {
	auto &rend = render->as<ChildWindowRender>();

	if (rend.font == nullptr) return;

	setTitle(title);

	auto barRect = getBarRect();

	DrawRectangleRec(rect, rend.bgColor);

	// title bar
	DrawRectangleRec(barRect, rend.secondaryColor);
	DrawRectangleLinesEx(barRect, rend.border, rend.currentBorderColor);

	// title
	float totalFontSize = rend.font->baseSize * rend.fontSize;
	Rectangle padding = paddingRect(barRect, rend.titlePadding);

	Vector2 textSize = MeasureTextEx(*rend.font, shownText.c_str(), totalFontSize, rend.spacing);
	Vector2 textPos;
	textPos.x = padding.x + Lerp(0.0f, padding.width - textSize.x, (static_cast<float>(rend.horiAlign) * 0.5f));
	textPos.y = padding.y + Lerp(0.0f, padding.height - textSize.y, (static_cast<float>(rend.vertAlign) * 0.5f));

	DrawTextEx(*rend.font, shownText.c_str(), textPos, totalFontSize, rend.spacing, rend.textColor);

	if (isScissor) {
		BeginScissorMode(contentRect.x, contentRect.y, contentRect.width, contentRect.height);
	}

	for (auto &widget : widgets) {
		widget->draw();
	}

	if (isScissor) EndScissorMode();

	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	closeButtonPtr->draw();
}

Rectangle ChildWindow::getContentRect() {
	return {rect.x, rect.y + EDUI_CHILDWINDOW_BARHEIGHT, rect.width, rect.height - EDUI_CHILDWINDOW_BARHEIGHT};
}

void ChildWindow::leftMouseClicked() {
	//dragging = true;
}

void ChildWindow::leftMouseReleased() {
	//dragging = false;
}
