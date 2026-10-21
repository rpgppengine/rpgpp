#ifndef _EDUI_TABBAR_H
#define _EDUI_TABBAR_H

#include <array>
#include <list>
#include <memory>
#include <string>

#include "edui/container.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconButton.hpp"
#include "edui/signal.hpp"

namespace edui {
struct TabBarRender : public HorizontalContainerRender {};

struct TabBar : public HorizontalContainer {
	SignalT<int> onActiveTabChanged;
	Signal onEmptied;

	std::weak_ptr<Container> contentBase;

	int size = 0;
	std::array<std::string, 20> tabTitles = {};
	std::list<std::shared_ptr<Container>> tabPages = {};

	std::shared_ptr<IconButton> leftButtonPtr = nullptr;
	std::shared_ptr<IconButton> rightButtonPtr = nullptr;

	TabBar();
	void applyTheme(const mINI::INIStructure &struc) override;
	void translate() override;

	void update() override;
	void draw() override;

	void updateContentRect() override;

	void leftMouseClicked() override;
	void leftMouseReleased() override;

	virtual std::shared_ptr<Container> addItem(const std::string &item, int iconId);
	void removeItem(int index);
	void showTabContent(int index);
	void showFirstTab();
	void showLastTab();

protected:
	int currentPage = 0;
};
}  // namespace edui

#endif
