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

void lua_entity_tween(EntityID entity, const std::string &componentName, std::vector<float> argsVec, float duration,
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

void lua_entity_tweenProp(EntityID entity, const std::string &componentName, const std::string &propName, float arg,
						  float duration, TweenType type) {
	auto variant = Game::getUi().getCurrentView()->getCoordinator().getComponentVariant(entity, componentName);
	auto prop = variant.get_type().get_raw_type().get_property(propName);

	if (prop.get_type().is_pointer() && prop.get_type().get_raw_type().is_arithmetic()) {
		float *f = prop.get_value(variant).get_value<float *>();
		float val = *f;
		Tween tween = {val, arg, f, duration, type};

		Game::getUi().getCurrentView()->addTween(tween);
	}
}

void lua_entity_tweenPropAlt(sol::object entity, const std::string &componentName, const std::string &propName,
							 sol::object arg, float duration, TweenType type) {
	sol::table entityTable = entity;
	sol::table componentTable = entityTable.get<sol::table>(componentName);

	sol::userdata userdata = componentTable[propName];
	sol::table meta = userdata[sol::metatable_key];

	sol::userdata varUser = arg;
	sol::table varMeta = varUser[sol::metatable_key];

	TweenContainer container;

	for (auto &tablePair : meta) {
		std::string key = tablePair.first.as<std::string>();
		auto val = tablePair.second;

		sol::object propObj = userdata.get<sol::object>(key);
		sol::object varPropObj = varUser.get<sol::object>(key);
		if (propObj.is<float>() && varPropObj.is<float>()) {
			TweenLua tween;
			tween.a = propObj.as<float>();
			tween.b = varPropObj.as<float>();
			tween.key = key;
			tween.dest = userdata;
			tween.source = varUser;
			tween.type = type;
			tween.duration = duration;

			container.addLuaTween(tween);
		}
	}

	Game::getUi().getCurrentView()->addTweenContainer(container);
}

sol::object lua_entity_getcomponent(EntityWrapper entity, const std::string &index) {
	bool exists = Game::getUi().getCurrentView()->getCoordinator().getComponents().componentNameExists(index);

	if (exists) {
		auto obj = Game::getUi().getCurrentView()->getCoordinator().getLuaObject(
			entity.entity, index, Game::getUi().getCurrentView()->getLuaEnvironment().lua_state());
		return obj;
	} else {
		return sol::object(sol::nil);
	}
}

EntityWrapper lua_view_getElement(InterfaceView *view, const std::string &title) {
	EntityID entity = view->getElement(title);
	return {entity};
}

void lua_ui_types_set(sol::environment &env) {
	env.new_usertype<EntityWrapper>(sol::no_construction(), sol::meta_function::index, lua_entity_getcomponent, "Tween",
									lua_entity_tween, "TweenProp", lua_entity_tweenProp, "TweenAlt",
									lua_entity_tweenPropAlt);

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
		sol::no_construction(), "GetEntity", &lua_view_getElement, "DeleteEntity", &InterfaceView::removeElement,
		"ScriptFile", sol::property(&InterfaceView::getScriptFile), "ChangeFocus",
		[](InterfaceView *view, const std::string &title) { view->changeFocusedElement(title); }, "Reset",
		&InterfaceView::resetElements);

	env.new_usertype<Rectangle>(
		"Rectangle",
		sol::factories([] { return Rectangle{}; },
					   [](float x, float y, float width, float height) { return Rectangle{x, y, width, height}; }),
		"x", &Rectangle::x, "y", &Rectangle::y, "width", &Rectangle::width, "height", &Rectangle::height);
	env.new_usertype<Color>("Color",
							sol::factories([] { return WHITE; }, [](unsigned char r, unsigned char g,
																	unsigned char b) { return Color{r, g, b, 255}; }),
							"r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a);
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
