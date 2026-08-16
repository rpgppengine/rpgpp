#ifndef _EDUI_TABBAR_H
#define _EDUI_TABBAR_H

#include <array>
#include <list>
#include <map>
#include <memory>
#include <string>

#include "edui/container.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/widget.hpp"
namespace edui {
struct TabBarRender : public HorizontalContainerRender {};

struct TabBar : public HorizontalContainer {
	std::weak_ptr<Container> contentBase;

	int size = 0;
	std::array<std::string, 20> tabTitles = {};
	std::list<std::shared_ptr<Container>> tabPages = {};

	std::shared_ptr<IconButton> leftButtonPtr = nullptr;
	std::shared_ptr<IconButton> rightButtonPtr = nullptr;

	int currentPage = 0;

	TabBar();

	void update() override;
	void draw() override;

	void updateContentRect() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

	virtual std::shared_ptr<Container> addItem(const std::string &item, int iconId);
	void removeItem(int index);
	void showTabContent(int index);

private:
	void processWidget(std::shared_ptr<Widget> &widget);
};
}  // namespace edui

#endif
