#include "edui/fileTabBar.hpp"

#include <memory>

#include "edui/fileTab.hpp"
#include "edui/tabBar.hpp"
#include "raymath.h"

using namespace edui;

FileTabBar::FileTabBar() : TabBar() { render = std::make_unique<FileTabBarRender>(); }

void FileTabBar::update() {
	for (auto &widget : widgets) {
		widget->as<FileTab>().closeEnabled = mouseIsInContent();
	}

	TabBar::update();

	if (scrollDebounce > 0.0f) {
		scrollDebounce--;
	}

	if (!notifiedMouseRelease && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
		holdingMouse = false;
		notifiedMouseRelease = true;

		if (notifiedMouseRelease) {
			onFileTabBarMouseRelease();
		}
	}

	if (holdingMouse) {
		Vector2 mousePos = GetMousePosition();
		if (scrollDebounce <= 0.0f) {
			if (mousePos.x < contentRect.x) {
				scrolled(0.5f);
				scrollDebounce = SCROLL_DEBOUNCE;
			} else if (mousePos.x > (contentRect.x + contentRect.width)) {
				scrolled(-0.5f);
				scrollDebounce = SCROLL_DEBOUNCE;
			}
		}

		if (auto sharedItem = heldItem.lock()) {
			auto offset = Vector2Subtract(mousePos, mouseOffset);

			sharedItem->layout.x.offset = (offset.x - this->contentRect.x) - scissorX;
		}
	}
}

void FileTabBar::draw() {
	TabBar::draw();

	if (holdingMouse) {
		if (auto sharedButton = heldItem.lock()) {
			sharedButton->draw();
		}
	}
}

std::shared_ptr<Container> FileTabBar::addItem(const std::string &item, int iconId) {
	if (size >= 20) return std::make_shared<Container>();

	int newReferId = size;

	auto content = std::make_shared<Container>();
	content->referId = newReferId;
	content->setSize({1, 0}, {1, 0});

	auto button = std::make_shared<FileTab>();
	button->tabId = newReferId;
	button->referId = newReferId;
	button->setSize({0, 0}, {1, 0});
	button->setText(item);
	button->iconId = iconId;
	button->calcRect(rect);

	std::weak_ptr<edui::FileTab> weakButton = button;

	button->onClicked.connect([this, newReferId, weakButton] {
		showTabContent(newReferId);

		holdingMouse = true;
		notifiedMouseRelease = false;
		if (auto sharedButton = weakButton.lock()) {
			heldItem = sharedButton;
			widgetRefer = sharedButton->referId;
			for (int i = 0; i < widgets.size(); i++) {
				if (widgets[i]->referId == widgetRefer) {
					heldIndex = i;
					break;
				}
			}

			Vector2 widgetAbsolutePos = {sharedButton->rect.x, sharedButton->rect.y};
			Vector2 mousePos = GetMousePosition();

			mouseOffset = Vector2Subtract(mousePos, widgetAbsolutePos);
		}
	});

	button->resizeToFit();
	add(button);
	showActiveTab(newReferId);

	if (auto sharedContentBase = contentBase.lock()) {
		tabPages.push_back(content);
		sharedContentBase->add(content);
		tabTitles[size] = item;
	}

	button->onDeleted.connect([this, newReferId] {
		removeItem(newReferId);
		updateWidgets();
		updateContentRect();
		fixScroll();
		size--;

		if (this->scissorX >= scrollMax) {
			scissorX = 0;
		}

		if (size == 0) return;
	});

	size++;

	return content;
}

void FileTabBar::showActiveTab(int index) {
	for (int i = 0; i < widgets.size(); i++) {
		FileTab &tab = widgets[i]->as<edui::FileTab>();
		if (i == index) {
			tab.setActive();
		} else {
			tab.setInactive();
		}
	}
}

void FileTabBar::onFileTabBarMouseRelease() {
	Vector2 barPos = {contentRect.x, contentRect.y};
	Vector2 mousePos = GetMousePosition();

	if (mousePos.x < contentRect.x) {
		mousePos.x += (contentRect.x - mousePos.x) + mouseOffset.x;
	} else if (mousePos.x > (contentRect.x + contentRect.width)) {
		mousePos.x -= (mousePos.x - (contentRect.x + contentRect.width)) + mouseOffset.x;
	}

	auto offset = Vector2Subtract(mousePos, barPos);
	offset.x -= scissorX;

	// calculate new index
	int res = heldIndex;
	if (auto sharedButton = heldItem.lock()) {
		for (size_t i = 0; i < widgets.size(); i++) {
			auto &widget = widgets[i];
			if (widget->mouseIsInRect() && widget->referId != sharedButton->referId) {
				res = i;
			}
		}
	}
	if (res >= widgets.size()) {
		res = widgets.size() - 1;
	}

	// reorder widgets
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
