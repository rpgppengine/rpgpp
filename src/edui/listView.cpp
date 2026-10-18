#include "edui/listView.hpp"

#include <cstdio>
#include <memory>

#include "edui/gui.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"
#include "raymath.h"

using namespace edui;

ListView::ListView() : VerticalContainer() { render = std::make_unique<ListViewRender>(); }

void ListView::update() {
	VerticalContainer::update();

	if (scrollDebounce > 0.0f) {
		scrollDebounce--;
	}

	if (!notifiedMouseRelease && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		holdingMouse = false;
		notifiedMouseRelease = true;

		if (notifiedMouseRelease) {
			onListReleaseMouse();
		}
	}

	if (holdingMouse) {
		Vector2 mousePos = GetMousePosition();
		if (scrollDebounce <= 0.0f) {
			if (mousePos.y < rect.y) {
				scrolled(0.5f);
				scrollDebounce = SCROLL_DEBOUNCE;
			} else if (mousePos.y > (rect.y + rect.height)) {
				scrolled(-0.5f);
				scrollDebounce = SCROLL_DEBOUNCE;
			}
		}

		if (auto sharedItem = heldItem.lock()) {
			auto offset = Vector2Subtract(mousePos, mouseOffset);

			sharedItem->layout.y.offset = (offset.y - this->rect.y) - scissorY;
		}
	}
}

void ListView::draw() {
	VerticalContainer::draw();

	if (holdingMouse) {
		if (auto sharedItem = heldItem.lock()) {
			sharedItem->draw();
		}
	}
}

void ListView::addItem(const std::string &title, int iconId) {
	auto item = std::make_shared<IconTextButton>();
	item->referId = widgets.size();
	item->setSize({1, 0}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	item->setText(title);
	item->iconId = iconId;

	std::weak_ptr<edui::IconTextButton> weakItem = item;

	item->onClicked.connect([this, weakItem] {
		holdingMouse = true;
		notifiedMouseRelease = false;
		if (auto sharedItem = weakItem.lock()) {
			heldItem = sharedItem;
			widgetRefer = sharedItem->referId;
			for (int i = 0; i < widgets.size(); i++) {
				if (widgets[i]->referId == widgetRefer) {
					heldIndex = i;
					break;
				}
			}

			Vector2 widgetAbsolutePos = {sharedItem->rect.x, sharedItem->rect.y};
			Vector2 mousePos = GetMousePosition();

			mouseOffset = Vector2Subtract(mousePos, widgetAbsolutePos);
		}
	});

	add(item);
}

void ListView::leftMouseReleased() {
	VerticalContainer::leftMouseReleased();
	holdingMouse = false;
}

void ListView::onListReleaseMouse() {
	Vector2 listPos = {rect.x, rect.y};
	Vector2 mousePos = GetMousePosition();

	if (mousePos.y < rect.y) {
		mousePos.y += (rect.y - mousePos.y) + mouseOffset.y;
	} else if (mousePos.y > (rect.y + rect.height)) {
		mousePos.y -= (mousePos.y - (rect.y + rect.height)) + mouseOffset.y;
	}

	auto offset = Vector2Subtract(mousePos, listPos);
	offset.y -= scissorY;

	int res = offset.y / EDUI_SECONDARY_HEIGHT;

	if (res >= widgets.size()) {
		res = widgets.size() - 1;
	}

	if (res < heldIndex) {
		std::shared_ptr<edui::Widget> next = widgets[res];
		for (int i = res + 1; i <= heldIndex; i++) {
			auto temp = widgets[i];
			widgets[i] = next;
			next = temp;
		}
		widgets[res] = next;
	} else if (res > heldIndex) {
		std::shared_ptr<edui::Widget> next = widgets[res];

		for (int i = res - 1; i >= heldIndex; i--) {
			auto temp = widgets[i];
			widgets[i] = next;
			next = temp;
		}
		widgets[res] = next;
	}

	updateWidgets();
}
