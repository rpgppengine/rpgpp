#include "edui/dropdown.hpp"

#include <memory>

#include "edui/dropdownList.hpp"
#include "edui/helper.hpp"
#include "edui/label.hpp"
#include "edui/rlicons.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

Dropdown::Dropdown() {
	render = std::make_unique<DropdownRender>();
	render->padding = 2;
	render->as<DropdownRender>().horiAlign = HorizontalAlignment::TEXT_LEFT;
	render->as<DropdownRender>().vertAlign = VerticalAlignment::TEXT_CENTER;
}

void Dropdown::setValue(const DropdownValue &val) { currentItem = val.idx; };

DropdownValue Dropdown::getValue() { return {currentItem, items[currentItem]}; };

void Dropdown::addItem(const std::string &item) {
	items[lastItem] = item;
	lastItem++;
}

void Dropdown::draw() {
	auto &rend = render->as<DropdownRender>();

	DrawRectangleRec(rect, rend.currentBgColor);
	DrawRectangleLinesEx(rect, rend.border, rend.currentBorderColor);

	if (rend.font == nullptr) return;

	auto paddingRect = getPaddingRect();
	drawOverflownText(paddingRect, rend.font, rend.fontSize, rend.spacing, items[currentItem], &shownText);

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;
	float spacing = rend.spacing > 0 ? rend.spacing : Gui::instance->fontSpacing;
	Vector2 textSize = MeasureTextEx(*rend.font, shownText.c_str(), totalFontSize, spacing);

	Vector2 textPos;
	textPos.x = paddingRect.x + Lerp(0.0f, paddingRect.width - textSize.x, (static_cast<float>(rend.horiAlign) * 0.5f));
	textPos.y =
		paddingRect.y + Lerp(0.0f, paddingRect.height - textSize.y, (static_cast<float>(rend.vertAlign) * 0.5f));

	DrawTextEx(*rend.font, shownText.c_str(), textPos, totalFontSize, spacing, rend.textColor);

	// draw arrow
	Rectangle iconRect = {rect.x + rect.width - (RAYGUI_ICON_SIZE), rect.y, RAYGUI_ICON_SIZE, RAYGUI_ICON_SIZE};
	Rectangle iconDestRect = {rect.x + (rect.width - rect.height), rect.y, rect.height, rect.height};
	rectCenter(iconDestRect, &iconRect);

	int icon = (opened ? ICON_ARROW_UP_FILL : ICON_ARROW_DOWN_FILL);
	GuiDrawIcon(icon, static_cast<int>(iconRect.x), static_cast<int>(iconRect.y), 1, rend.currentBorderColor);
}

void Dropdown::leftMouseClicked() {
	if (!opened) {
		openDropdown();
	} else {
		closeDropdown();
	}

	Widget::leftMouseClicked();
}

void Dropdown::openDropdown() {
	auto &rend = render->as<DropdownRender>();

	Vector2 listPos = {static_cast<float>(layout.x.offset), layout.y.offset + rect.height};

	float totalFontSize = rend.fontSize > 0 ? rend.fontSize : Gui::instance->labelFontSize;

	auto list = std::make_shared<edui::DropdownList>();
	list->setPosition({0, static_cast<int>(listPos.x)}, {0, static_cast<int>(listPos.y)});
	list->setSize({0, static_cast<int>(rect.width)}, {0, EDUI_DROPDOWNLIST_HEIGHT});

	Gui::instance->addTop(list);

	for (int i = 0; i < lastItem; i++) {
		list->addItem(items[i], rend.fontSize);
	}

	list->onValueChanged.connect([this](int idx) {
		edui::DropdownValue previous = {currentItem, items[currentItem]};
		edui::DropdownValue current = {idx, items[idx]};
		onValueChangedT.invoke(previous, current);

		currentItem = idx;
		opened = false;
	});
	list->onDeleted.connect([this] {
		opened = false;
		deferFlag = true;
	});

	this->listPtr = list;

	opened = true;
}

void Dropdown::closeDropdown() {
	if (auto list = listPtr.lock()) {
		list->markDelete();
		opened = false;
	}
}
