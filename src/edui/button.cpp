#include "edui/button.hpp"
#include <memory>
#include "edui/label.hpp"
#include "raylib.h"

using namespace edui;

Button::Button() : Label() {
	render = std::make_unique<ButtonRender>();
	render->padding = 8;
	render->focusBgColor = LIGHTGRAY;
}
