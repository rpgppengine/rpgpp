#include "edui/tabBar.hpp"

#include <memory>
#include <string>

#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/scrollableContainer.hpp"
#include "edui/tab.hpp"
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

	scrollbar->visible = false;
}

void TabBar::applyTheme(const mINI::INIStructure &struc) {
	ScrollableContainer::applyTheme(struc);

	leftButtonPtr->applyTheme(struc);
	leftButtonPtr->unfocused();

	rightButtonPtr->applyTheme(struc);
	rightButtonPtr->unfocused();
}

void TabBar::translate() {
	for (auto &widget : widgets) {
		std::string widgetTranslationId =
			std::string(translationId.c_str()) + '.' + std::string(widget->translationId.c_str());
		std::string widgetText = getTranslation(widgetTranslationId, widget->as<edui::Tab>().getText().c_str());
		widget->as<edui::Tab>().setText(widgetText);
		widget->as<edui::Tab>().resizeToFit();
	}
}

void TabBar::update() {
	HorizontalContainer::update();

	leftButtonPtr->calcRect(rect);
	rightButtonPtr->calcRect(rect);

	leftButtonPtr->update();
	rightButtonPtr->update();

	std::shared_ptr<Widget> leftWidget = leftButtonPtr;
	processPseudoChild(gui, leftWidget);

	std::shared_ptr<Widget> rightWidget = rightButtonPtr;
	processPseudoChild(gui, rightWidget);
}

void TabBar::draw() {
	ScrollableContainer::draw();
	if (overflown) {
		leftButtonPtr->draw();
		rightButtonPtr->draw();
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

	int newReferId = size;

	auto content = std::make_shared<Container>();
	content->referId = newReferId;

	auto button = std::make_shared<Tab>();
	button->translationId = item.c_str();
	button->referId = newReferId;
	button->setSize({0, 0}, {1, 0});
	button->setText(item);
	button->iconId = iconId;
	button->calcRect(rect);

	button->onClicked.connect([this, newReferId] {
		showTabContent(newReferId);
		onActiveTabChanged.invoke(newReferId);
	});

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
		updateWidgets();
		updateContentRect();
		fixScroll();

		if (this->scissorX > scrollMax) {
			scissorX = 0;
		}
	});

	return content;
}

void TabBar::removeItem(int index) {
	// remove the page content
	for (auto pageIt = tabPages.begin(); pageIt != tabPages.end();) {
		if (pageIt->get()->referId == index) {
			pageIt->get()->markDelete();
			pageIt = tabPages.erase(pageIt);
			break;
		} else {
			pageIt++;
		}
	}

	// set new active page
	if (!widgets.empty()) {
		int newReferId = -1;
		for (auto tabIt = widgets.begin(); tabIt != widgets.end();) {
			if (!tabIt->get()->deleteFlag) {
				newReferId = tabIt->get()->referId;
				break;
			} else {
				tabIt++;
			}
		}

		if (newReferId >= 0) {
			showTabContent(newReferId);
		} else {
			onEmptied.invoke();
		}
	} else {
		onEmptied.invoke();
	}
}

void TabBar::showTabContent(int index) {
	for (auto tabIt = widgets.begin(); tabIt != widgets.end();) {
		if (tabIt->get()->referId == index) {
			tabIt->get()->as<Tab>().setActive();
			tabIt->get()->as<Tab>().unfocused();
		} else {
			tabIt->get()->as<Tab>().setInactive();
			tabIt->get()->as<Tab>().unfocused();
		}

		tabIt++;
	}

	for (auto &page : tabPages) {
		int referId = page->referId;

		if (referId == index) {
			page->visible = true;
			currentPage = index;
		} else {
			page->visible = false;
		}
	}

	onActiveTabChanged.invoke(index);
}

void TabBar::showFirstTab() {
	auto it = tabPages.begin();
	int referId = it->get()->referId;
	showTabContent(referId);
}

void TabBar::showLastTab() {
	auto it = tabPages.rbegin();
	int referId = it->get()->referId;
	showTabContent(referId);
}
