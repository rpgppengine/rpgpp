#include "lua/uiTypesApi.hpp"

#include "component.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "lua.h"
#include "raylib.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "sol/environment.hpp"
#include "sol/forward.hpp"
#include "sol/inheritance.hpp"
#include "sol/object.hpp"
#include "sol/property.hpp"
#include "sol/raii.hpp"
#include "sol/stack_core.hpp"
#include "sol/types.hpp"

sol::object lua_view_getElement(InterfaceView *view, const std::string &title) {
	printf("%s \n", title.c_str());

	EntityID entity = view->getElement(title);
	auto &ecs = view->getCoordinator();

	sol::environment &env = Game::getUi().getCurrentView()->getLuaEnvironment();

	sol::table tbl = env.create();

	auto set = ecs.getEntityComponents(entity);
	for (auto &name : set) {
		tbl[name] = ecs.getLuaObject(entity, name, env.lua_state());
	}

	return tbl;
}

void lua_ui_types_set(sol::state_view lua) {
	lua.new_usertype<InterfaceView>(sol::no_construction(), "getEntity", &lua_view_getElement, "deleteEntity",
									&InterfaceView::removeElement, "scriptFile", &InterfaceView::getScriptFile);

	lua.new_usertype<Rectangle>("Rectangle", "x", &Rectangle::x, "y", &Rectangle::y, "width", &Rectangle::width,
								"height", &Rectangle::height);
	lua.new_usertype<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
	lua.new_enum("NPatchLayout", "NinePatch", NPATCH_NINE_PATCH, "ThreePatchHorizontal", NPATCH_THREE_PATCH_HORIZONTAL,
				 "ThreePatchVertical", NPATCH_THREE_PATCH_VERTICAL);
	lua.new_usertype<FontRef>("Font", "Source", &FontRef::path, "FontSize", &FontRef::fontSize);
	lua.new_usertype<ImageRef>("Image", "Source", &ImageRef::path, "Scale", &ImageRef::scale);
	lua.new_usertype<NPatchInfo>("NPatchInfo", "Top", &NPatchInfo::top, "Bottom", &NPatchInfo::bottom, "Left",
								 &NPatchInfo::left, "Right", &NPatchInfo::right);
	lua.new_enum("TextAlignment", "Top", TEXT_ALIGN_BOTTOM, "Center", TEXT_ALIGN_CENTRE, "Bottom", TEXT_ALIGN_BOTTOM,
				 "Left", TEXT_ALIGN_LEFT, "Middle", TEXT_ALIGN_MIDDLE, "Right", TEXT_ALIGN_RIGHT);
	lua.new_usertype<UIElementRef>("UIElementRef", "Id", &UIElementRef::entityId);

	lua.new_usertype<VisibilityComponent>("VisibilityComponent", "IsVisible", &VisibilityComponent::isVisible);
	lua.new_usertype<InputComponent>("InputComponent", "up", &InputComponent::upButton, "down",
									 &InputComponent::downButton, "left", &InputComponent::leftButton, "right",
									 &InputComponent::rightButton);
	lua.new_usertype<LabelComponent>("LabelComponent", "Text", &LabelComponent::text, "TextColor",
									 &LabelComponent::textColor, "HorizontalAlignment",
									 &LabelComponent::horizontalAlignment, "VerticalAlignment",
									 &LabelComponent::verticalAlignment, "Font", &LabelComponent::font);
	lua.new_usertype<TextAreaComponent>("TextAreaComponent", "Text", &TextAreaComponent::text, "TextArea",
										&TextAreaComponent::textColor, "Font", &TextAreaComponent::font);
	lua.new_usertype<ColorRectComponent>("ColorRectComponent", "Color", &ColorRectComponent::color);
	lua.new_usertype<ImageRectComponent>("ImageRectComponent", "Source", &ImageRectComponent::image);
	lua.new_usertype<NinePatchImageRectComponent>("NinePatchRectComponent", "Source",
												  &NinePatchImageRectComponent::image, "NPatchInfo",
												  &NinePatchImageRectComponent::npatchInfo);
	lua.new_usertype<DialogueComponent>("DialogueComponent", "Text", &DialogueComponent::text, "TextColor",
										&DialogueComponent::textColor);
	lua.new_usertype<ButtonComponent>("ButtonComponent", "normalTextColor", &ButtonComponent::normalTextColor,
									  "focusedTextColor", &ButtonComponent::focusedTextColor);
}

void lua_ui_types_set(sol::environment &env) {
	env.new_usertype<InterfaceView>(sol::no_construction(), "getEntity", lua_view_getElement, "deleteEntity",
									&InterfaceView::removeElement, "scriptFile", &InterfaceView::getScriptFile);

	env.new_usertype<Rectangle>("Rectangle", "x", &Rectangle::x, "y", &Rectangle::y, "width", &Rectangle::width,
								"height", &Rectangle::height);
	env.new_usertype<Color>("Color", "r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
	env.new_enum("NPatchLayout", "NinePatch", NPATCH_NINE_PATCH, "ThreePatchHorizontal", NPATCH_THREE_PATCH_HORIZONTAL,
				 "ThreePatchVertical", NPATCH_THREE_PATCH_VERTICAL);
	env.new_usertype<FontRef>("Font", "Source", &FontRef::path, "FontSize", &FontRef::fontSize);
	env.new_usertype<ImageRef>("Image", "Source", &ImageRef::path, "Scale", &ImageRef::scale);
	env.new_usertype<NPatchInfo>("NPatchInfo", "Top", &NPatchInfo::top, "Bottom", &NPatchInfo::bottom, "Left",
								 &NPatchInfo::left, "Right", &NPatchInfo::right);
	env.new_enum("TextAlignment", "Top", TEXT_ALIGN_BOTTOM, "Center", TEXT_ALIGN_CENTRE, "Bottom", TEXT_ALIGN_BOTTOM,
				 "Left", TEXT_ALIGN_LEFT, "Middle", TEXT_ALIGN_MIDDLE, "Right", TEXT_ALIGN_RIGHT);
	env.new_usertype<UIElementRef>("UIElementRef", "Id", &UIElementRef::entityId);

	env.new_usertype<VisibilityComponent>("VisibilityComponent", "IsVisible", &VisibilityComponent::isVisible);
	env.new_usertype<InputComponent>("InputComponent", "up", &InputComponent::upButton, "down",
									 &InputComponent::downButton, "left", &InputComponent::leftButton, "right",
									 &InputComponent::rightButton);
	env.new_usertype<LabelComponent>("LabelComponent", "Text", &LabelComponent::text, "TextColor",
									 &LabelComponent::textColor, "HorizontalAlignment",
									 &LabelComponent::horizontalAlignment, "VerticalAlignment",
									 &LabelComponent::verticalAlignment, "Font", &LabelComponent::font);
	env.new_usertype<TextAreaComponent>("TextAreaComponent", "Text", &TextAreaComponent::text, "TextArea",
										&TextAreaComponent::textColor, "Font", &TextAreaComponent::font);
	env.new_usertype<ColorRectComponent>("ColorRectComponent", "Color", &ColorRectComponent::color);
	env.new_usertype<ImageRectComponent>("ImageRectComponent", "Source", &ImageRectComponent::image);
	env.new_usertype<NinePatchImageRectComponent>("NinePatchRectComponent", "Source",
												  &NinePatchImageRectComponent::image, "NPatchInfo",
												  &NinePatchImageRectComponent::npatchInfo);
	env.new_usertype<DialogueComponent>("DialogueComponent", "Text", &DialogueComponent::text, "TextColor",
										&DialogueComponent::textColor);
	env.new_usertype<ButtonComponent>("ButtonComponent", "normalTextColor", &ButtonComponent::normalTextColor,
									  "focusedTextColor", &ButtonComponent::focusedTextColor);
}