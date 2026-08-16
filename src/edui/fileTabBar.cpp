#include "edui/fileTabBar.hpp"
#include <memory>
#include "edui/fileTab.hpp"
#include "edui/tabBar.hpp"

using namespace edui;

FileTabBar::FileTabBar() : TabBar() {
	render = std::make_unique<FileTabBarRender>();
}

std::shared_ptr<Container> FileTabBar::addItem(const std::string &item, int iconId) {
	if (size >= 20) return std::make_shared<Container>();

	auto content = std::make_shared<Container>();
	content->referId = size;

	auto button = std::make_shared<FileTab>();
	button->tabId = size;
	button->setSize({0, 0}, {1, 0});
	button->setText(item);
	button->iconId = iconId;
	button->calcRect(rect);

	int curr = size;
	button->clicked = [this, curr] { showTabContent(curr); };

	button->resizeToFit();
	add(button);

	content->setSize({1, 0}, {1, 0});
	if (auto sharedContentBase = contentBase.lock()) {
		tabPages.push_back(content);
		sharedContentBase->add(content);
		tabTitles[size] = item;
	}

	button->onDeleted = [this, curr] {
		removeItem(curr);
		updateContentRect();
		size--;

		if (this->scissorX >= scrollMax) {
			scissorX = 0;
		}

		if (currentPage == curr) {
			showTabContent(tabPages.cbegin()->get()->referId);
		}
	};

	size++;

	return content;
}
