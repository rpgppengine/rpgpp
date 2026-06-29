#include "lua/uiTypesApi.hpp"

#include <cstdio>

#include "component.hpp"
#include "entity.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "lua.h"
#include "raylib.h"
#include "reflect.hpp"
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
#include "tween.hpp"

void lua_entity_addcomponent(EntityID entity, const std::string &componentName) {
	Game::getUi().getCurrentView()->getCoordinator().insertEmptyComponent(entity, componentName);
}

void lua_entity_addtween(EntityID entity, const std::string &componentName, std::vector<float> argsVec, float duration,
						 TweenType type) {
	int i = 0;

	auto &tweens = Game::getUi().getCurrentView()->getTweens();
	auto &container = tweens.emplace_back();
	auto variant = Game::getUi().getCurrentView()->getCoordinator().getComponentVariant(entity, componentName);
	printf("%s \n", variant.get_type().get_name().to_string().c_str());
	for (auto &prop : variant.get_type().get_raw_type().get_properties()) {
		printf("%s \n", prop.get_name().to_string().c_str());
		if (prop.get_type().is_pointer() && prop.get_type().get_raw_type().is_arithmetic()) {
			float *f = prop.get_value(variant).get_value<float *>();
			float val = *f;
			Tween tween = {val, argsVec[i], f, duration, type};
			container.addTween(tween);

			i++;
		}
	}
}

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

	tbl.set_function("AddTween",
					 [entity](const std::string &componentName, std::vector<float> argsVec, float duration,
							  TweenType type) { lua_entity_addtween(entity, componentName, argsVec, duration, type); });

	return tbl;
}

void lua_ui_types_set(sol::environment &env) {
	env.new_enum("TweenType", "Linear", TweenType::LINEAR, "InSine", TweenType::INSINE, "OutSine", TweenType::OUTSINE,
				 "InOutSine", TweenType::INOUTSINE, "InQuad", TweenType::INQUAD, "OutQuad", TweenType::OUTQUAD,
				 "InOutQuad", TweenType::INOUTQUAD, "InCubic", TweenType::INCUBIC, "OutCubic", TweenType::OUTCUBIC,
				 "InOutCubic", TweenType::INOUTCUBIC, "InQuart", TweenType::INQUART, "OutQuart", TweenType::OUTQUART,
				 "InOutQuart", TweenType::INOUTQUART, "InQuint", TweenType::INQUINT, "OutQuint", TweenType::OUTQUINT,
				 "InOutQuint", TweenType::INOUTQUINT, "InExpo", TweenType::INEXPO, "OutExpo", TweenType::OUTEXPO,
				 "InOutExpo", TweenType::INOUTEXPO, "InCirc", TweenType::INCIRC, "OutCirc", TweenType::OUTCIRC,
				 "InOutCirc", TweenType::INOUTCIRC, "InBack", TweenType::INBACK, "OutBack", TweenType::OUTBACK,
				 "InOutBack", TweenType::INOUTBACK, "InElastic", TweenType::INELASTIC, "OutElastic",
				 TweenType::OUTELASTIC, "InOutElastic", TweenType::INOUTELASTIC, "InBounce", TweenType::INBOUNCE,
				 "OutBounce", TweenType::OUTBOUNCE, "InOutBounce", TweenType::INOUTBOUNCE);

	env.new_usertype<InterfaceView>(
		sol::no_construction(), "GetEntity", &lua_view_getElement, "DeleteEntity", &InterfaceView::removeElement,
		"ScriptFile", sol::property(&InterfaceView::getScriptFile), "ChangeFocus",
		[](InterfaceView *view, const std::string &title) { view->changeFocusedElement(title); });

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
