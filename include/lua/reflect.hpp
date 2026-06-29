#ifndef _RPGPP_REFLECT_H
#define _RPGPP_REFLECT_H

#include <rttr/registration.h>

#include "component.hpp"
#include "gamedata.hpp"
#include "raylib.h"
#include "rttr/detail/registration/registration_impl.h"
#include "rttr/policy.h"

RTTR_REGISTRATION {
	rttr::registration::enumeration<CallbackType>("CallbackType")(rttr::value("callback_focused", CALLBACK_FOCUSED),
																  rttr::value("callback_unfocused", CALLBACK_UNFOCUSED),
																  rttr::value("callback_trigger", CALLBACK_TRIGGER));

	rttr::registration::class_<Rectangle>("Rectangle")
		.constructor<>()
		.property("x", &Rectangle::x)(rttr::policy::prop::bind_as_ptr)
		.property("y", &Rectangle::y)(rttr::policy::prop::bind_as_ptr)
		.property("width", &Rectangle::width)(rttr::policy::prop::bind_as_ptr)
		.property("height", &Rectangle::height)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<VisibilityComponent>("VisibilityComponent")
		.constructor<>()
		.property("is_visible", &VisibilityComponent::isVisible)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<InputComponent>("InputComponent")
		.constructor<>()
		.property("upButton", &InputComponent::upButton)(rttr::policy::prop::bind_as_ptr)
		.property("downButton", &InputComponent::downButton)(rttr::policy::prop::bind_as_ptr)
		.property("leftButton", &InputComponent::leftButton)(rttr::policy::prop::bind_as_ptr)
		.property("rightButton", &InputComponent::rightButton)(rttr::policy::prop::bind_as_ptr)
		.property("callbacks", &InputComponent::funcNames)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<LabelComponent>("LabelComponent")
		.constructor<>()
		.property("text", &LabelComponent::text)(rttr::policy::prop::bind_as_ptr)
		.property("textColor", &LabelComponent::textColor)(rttr::policy::prop::bind_as_ptr)
		.property("horizontalAlignment", &LabelComponent::horizontalAlignment)(rttr::policy::prop::bind_as_ptr)
		.property("verticalAlignment", &LabelComponent::verticalAlignment)(rttr::policy::prop::bind_as_ptr)
		.property("font", &LabelComponent::font)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<TextAreaComponent>("TextAreaComponent")
		.constructor<>()
		.property("text", &TextAreaComponent::text)(rttr::policy::prop::bind_as_ptr)
		.property("textColor", &TextAreaComponent::textColor)(rttr::policy::prop::bind_as_ptr)
		.property("font", &TextAreaComponent::font)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ColorRectComponent>("ColorRectComponent")
		.constructor<>()
		.property("color", &ColorRectComponent::color)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ImageRectComponent>("ImageRectComponent")
		.constructor<>()
		.property("source", &ImageRectComponent::image)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<NinePatchImageRectComponent>("NinePatchImageRectComponent")
		.constructor<>()
		.property("source", &NinePatchImageRectComponent::image)(rttr::policy::prop::bind_as_ptr)
		.property("npatchinfo", &NinePatchImageRectComponent::npatchInfo)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<DialogueComponent>("DialogueComponent")
		.constructor<>()
		.property("text", &DialogueComponent::text)(rttr::policy::prop::bind_as_ptr)
		.property("textColor", &DialogueComponent::textColor)(rttr::policy::prop::bind_as_ptr)
		.property("defaultFont", &DialogueComponent::defaultFont)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ButtonComponent>("ButtonComponent")
		.constructor<>()
		.property("normalTextColor", &ButtonComponent::normalTextColor)(rttr::policy::prop::bind_as_ptr)
		.property("focusedTextColor", &ButtonComponent::focusedTextColor)(rttr::policy::prop::bind_as_ptr);
}

#endif