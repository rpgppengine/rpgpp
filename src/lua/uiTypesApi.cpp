#include "lua/uiTypesApi.hpp"

#include "button.hpp"
#include "component.hpp"
#include "lua.h"
#include "raylib.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "sol/forward.hpp"
#include "sol/inheritance.hpp"
#include "sol/object.hpp"
#include "sol/property.hpp"
#include "sol/raii.hpp"
#include "sol/stack_core.hpp"
#include "sol/types.hpp"
#include "textArea.hpp"
#include "uiElement.hpp"

void lua_ui_types_set(sol::state_view lua) {
	lua.new_usertype<Rectangle>("Rectangle", "x", &Rectangle::x, "y", &Rectangle::y, "width", &Rectangle::width,
								"height", &Rectangle::height);
	lua.new_usertype<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
	lua.new_usertype<ColorRectComponent>("ColorRectComponent", "color", &ColorRectComponent::color);
}