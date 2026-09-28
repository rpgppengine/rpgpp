#include "edui/tab.hpp"

#include <memory>

#include "edui/iconTextButton.hpp"
#include "edui/widget.hpp"

using namespace edui;

Tab::Tab() : IconTextButton() {
	render = std::make_unique<TabRender>();
	render->padding = 2;
	auto &rend = render->as<TabRender>();
	rend.vertAlign = VerticalAlignment::TEXT_CENTER;
}

void Tab::setActive() {
	auto &rend = render->as<TabRender>();
	rend.bgColor = rend.activeColor;
	rend.focusBgColor = rend.activeFocusColor;
}

void Tab::setInactive() {
	auto &rend = render->as<TabRender>();
	rend.bgColor = rend.inactiveColor;
	rend.focusBgColor = rend.inactiveFocusColor;
}
