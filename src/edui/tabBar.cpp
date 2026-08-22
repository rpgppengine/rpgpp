#include "edui/tabBar.hpp"

#include <memory>
#include <string>

#include "edui/container.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

TabBar::TabBar() : HorizontalContainer() {
	render = std::make_unique<TabBarRender>();

	leftButtonPtr = std::make_shared<IconButton>();
	leftButtonPtr->setPosition({1, 0}, {0, 0});
	leftButtonPtr->setSize({0, 0}, {1, 0});
	leftButtonPtr->iconId = ICON_ARROW_LEFT_FILL;

	rightButtonPtr = std::make_shared<IconButton>();
	rightButtonPtr->setPosition({1, 0}, {0, 0});
	rightButtonPtr->setSize({0, 0}, {1, 0});
	rightButtonPtr->iconId = ICON_ARROW_RIGHT_FILL;

	leftButtonPtr->onClicked.connect([this] { scrolled(1.0f); });
	rightButtonPtr->onClicked.connect([this] { scrolled(-1.0f); });
}

void TabBar::processWidget(std::shared_ptr<Widget> &widget) {
	if (widget->mouseIsInRect()) {
		Gui::instance->notifyChild(&widget);
	} else {
		if (widget->notifiedMouseEnter) {
			widget->notifiedMouseEnter = false;
			widget->mouseLeft();
		}
	}
}

void TabBar::update() {
	HorizontalContainer::update();

	leftButtonPtr->calcRect(rect);
	rightButtonPtr->calcRect(rect);
}

void TabBar::draw() {
	ScrollableContainer::draw();
	if (overflown) {
		DrawRectangleRec(scrollAreaRect, GREEN);

		leftButtonPtr->draw();
		rightButtonPtr->draw();

		std::shared_ptr<Widget> leftWidget = leftButtonPtr;
		processWidget(leftWidget);

		std::shared_ptr<Widget> rightWidget = rightButtonPtr;
		processWidget(rightWidget);
	}
}

void TabBar::updateContentRect() {
	HorizontalContainer::updateContentRect();

	this->contentRect = rect;
	if (overflown) {
		float old = contentRect.height;
		contentRect.width -= (rect.height * 2);
	}
	renderRect = paddingRect(contentRect, render->padding);
	if (isScissor) {
		renderRect.x += scissorX;
		renderRect.y += scissorY;
	}

	scrollAreaRect = {rect.x + contentRect.width, rect.y, rect.height * 2, rect.height};

	float content = contentRect.width;
	float scissor = scissorRect.width;
	scrollMax = (-scissor + content);

	if (scissorRect.width > contentRect.width) {
		overflown = true;
	} else {
		overflown = false;
	}

	if (overflown) {
		leftButtonPtr->layout.x.offset = -scrollAreaRect.width;
		leftButtonPtr->layout.width.offset = rect.height;

		rightButtonPtr->layout.x.offset = -rect.height;
		rightButtonPtr->layout.width.offset = rect.height;
	}
}

void TabBar::leftMouseClicked() {}

void TabBar::leftMouseReleased() {}

std::shared_ptr<Container> TabBar::addItem(const std::string &item, int iconId) {
	if (size >= 20) return std::make_shared<Container>();

	auto content = std::make_shared<Container>();
	content->referId = size;

	auto button = std::make_shared<IconTextButton>();
	button->referId = size;
	button->setSize({0, 0}, {1, 0});
	button->setText(item);
	button->iconId = iconId;
	button->calcRect(rect);

	int curr = size;
	button->onClicked.connect([this, curr] { showTabContent(curr); });

	button->resizeToFit();
	add(button);

	content->setSize({1, 0}, {1, 0});
	if (auto sharedContentBase = contentBase.lock()) {
		tabPages.push_back(content);
		sharedContentBase->add(content);
		tabTitles[size] = item;
	}

	size++;

	button->onDeleted.connect([this] {
		updateContentRect();

		if (this->scissorX > scrollMax) {
			scissorX = 0;
		}
	});

	return content;
}

void TabBar::removeItem(int index) {
	printf("%i \n", index);
	if (index >= (size - 1)) return;

	int i = index;
	tabTitles[index] = "";
	auto &elem = tabTitles[i + 1];
	while (!elem.empty()) {
		tabTitles[i].swap(elem);

		i++;
		elem = tabTitles[i + 1];
	}

	printf("===\n");

	i = 0;
	for (auto it = tabPages.begin(); it != tabPages.end();) {
		printf("%i \n", i);
		if (it->get()->referId == index) {
			it->get()->markDelete();
			it = tabPages.erase(it);
			break;
		} else {
			it++;
			i++;
		}
	}

	for (auto it = tabPages.begin(); it != tabPages.end();) {
		printf("widgets count: %zu\n", it->get()->widgets.size());

		it++;
	}
}

void TabBar::showTabContent(int index) {
	int i = 0;
	for (auto it = tabPages.begin(); it != tabPages.end();) {
		if (it->get()->referId == index) {
			printf("shown %i \n", index);
			it->get()->visible = true;
			currentPage = index;
		} else {
			it->get()->visible = false;
		}

		it++;
		i++;
	}
}
