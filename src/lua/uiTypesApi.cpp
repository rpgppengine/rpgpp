#include "lua/uiTypesApi.hpp"

#include <cstdio>

#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "lua.hpp"
#include "raylib.h"
#include "sol/environment.hpp"
#include "sol/forward.hpp"
#include "sol/inheritance.hpp"
#include "sol/object.hpp"
#include "sol/property.hpp"
#include "sol/raii.hpp"
#include "sol/stack_core.hpp"
#include "sol/types.hpp"
#include "tween.hpp"

void lua_element_tween(UIElement *element, const std::string &propName, sol::object val, float duration,
					   TweenType type) {
	auto destVar = &element->props[propName];
	auto sourceVar = val.as<ElementProperty>();

	if (destVar->index() != sourceVar.index()) {
		return;
	}

	auto resList = TweenProvider::initPropTween(destVar, sourceVar, duration, type);
	TweenContainer cont;
	for (auto &tween : resList) {
		cont.addTween(tween);
	}
	Game::getUi().getCurrentView()->addTweenContainer(cont);
}

sol::object lua_view_getElement(InterfaceView *view, const std::string &title) {
	auto state = Game::getUi().getCurrentView()->getLuaEnvironment().lua_state();
	return sol::make_object(state, view->getElement(title));
}

sol::object lua_element_index(UIElement *element, const std::string &prop) {
	auto state = Game::getUi().getCurrentView()->getLuaEnvironment().lua_state();
	if (element->props.count(prop) > 0) {
		return sol::make_object(state, element->props[prop]);
	} else {
		return sol::object(sol::nil);
	}
}

void lua_element_newindex(UIElement *element, const std::string &prop, sol::object value) {
	if (element->props.count(prop) > 0 && value.is<ElementProperty>()) {
		auto variant = value.as<ElementProperty>();
		if (element->props[prop].index() == variant.index()) {
			element->props[prop] = variant;
		}
	}
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

	env.new_usertype<Tween>("Tween", sol::constructors<Tween()>(), "Start", &Tween::a, "End", &Tween::b, "Duration",
							&Tween::duration, "Type", &Tween::type, "Ptr", &Tween::ptr);

	env.new_usertype<TweenContainer>("TweenContainer", sol::constructors<TweenContainer()>(), "AddTween",
									 &TweenContainer::addTween);

	env.new_usertype<InterfaceView>(
		sol::no_construction(), "GetEntity", lua_view_getElement, "Reset", &InterfaceView::resetElements, "ChangeFocus",
		[](InterfaceView *view, const std::string &title) { view->changeFocusedElement(title); });

	env.new_usertype<UIElement>(sol::no_construction(), sol::meta_function::index, lua_element_index,
								sol::meta_function::new_index, lua_element_newindex, "Tween", lua_element_tween);
}
