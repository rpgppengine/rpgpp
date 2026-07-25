#include "edui/button.hpp"
#include <memory>

using namespace edui;

Button::Button() {
	render = std::make_unique<ButtonRender>();
	render->padding = 8;
	render->focusBgColor = {220, 220, 220, 255};
}
