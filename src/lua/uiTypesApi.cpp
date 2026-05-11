#include "lua/uiTypesApi.hpp"

#include "button.hpp"
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
	/*
	lua.new_usertype<UIElement>(sol::no_construction(), "Rect",
								sol::property(&UIElement::getRect, &UIElement::setRect));

	lua.new_usertype<TextArea>("TextArea", sol::constructors<TextArea(), TextArea(Rectangle)>(), "Text",
							   &TextArea::content, sol::base_classes, sol::bases<UIElement>());

	lua.new_usertype<Button>("Button", sol::constructors<Button(), Button(Rectangle)>(), "Text",
							 sol::property(&Button::getText, &Button::setText), sol::base_classes,
							 sol::bases<UIElement>());

	auto uiElement = lua.new_usertype<UIElement>(sol::no_construction());
	*/

	auto *L = lua.lua_state();
	for (auto &rType : rttr::type::get_types()) {
		if (rType.is_class()) {
			sol::table t = lua.create_named_table(rType.get_name().to_string());

			sol::stack::push_userdata(L, t);
		}
	}
}