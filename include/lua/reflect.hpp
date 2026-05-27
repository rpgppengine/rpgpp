#ifndef _RPGPP_REFLECT_H
#define _RPGPP_REFLECT_H

#include <rttr/registration.h>

#include "component.hpp"
#include "raylib.h"
#include "rttr/detail/registration/registration_impl.h"
#include "rttr/policy.h"

RTTR_REGISTRATION {
	rttr::registration::class_<Rectangle>("Rectangle").constructor<>();
	rttr::registration::class_<InputComponent>("InputComponent")
		.constructor<>()
		.property("up", &InputComponent::upButton)
		.property("down", &InputComponent::downButton)
		.property("left", &InputComponent::leftButton)
		.property("right", &InputComponent::rightButton)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<LabelComponent>("LabelComponent")
		.constructor<>()
		.property("text", &LabelComponent::text)
		.property("textColor", &LabelComponent::textColor)
		.property("horizontalAlignment", &LabelComponent::horizontalAlignment)
		.property("verticalAlignment", &LabelComponent::verticalAlignment)
		.property("fontName", &LabelComponent::fontName)
		.property("fontSize", &LabelComponent::fontSize)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<TextAreaComponent>("TextAreaComponent")
		.constructor<>()
		.property("text", &TextAreaComponent::text)
		.property("textColor", &TextAreaComponent::textColor)
		.property("fontName", &TextAreaComponent::fontName)
		.property("fontSize", &TextAreaComponent::fontSize)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ColorRectComponent>("ColorRectComponent")
		.constructor<>()
		.property("color", &ColorRectComponent::color)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ImageRectComponent>("ImageRectComponent")
		.constructor<>()
		.property("source", &ImageRectComponent::source)
		.property("scale", &ImageRectComponent::scale)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<NinePatchImageRectComponent>("NinePatchImageRectComponent")
		.constructor<>()
		.property("source", &NinePatchImageRectComponent::source)
		.property("scale", &NinePatchImageRectComponent::scale)
		.property("npatchinfo", &NinePatchImageRectComponent::npatchInfo)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<DialogueComponent>("DialogueComponent")
		.constructor<>()
		.property("text", &DialogueComponent::text)
		.property("textColor", &DialogueComponent::textColor)
		.property("defaultFont", &DialogueComponent::defaultFont)(rttr::policy::prop::bind_as_ptr);
	rttr::registration::class_<ButtonComponent>("ButtonComponent")
		.constructor<>()
		.property("normalTextColor", &ButtonComponent::normalTextColor)
		.property("focusedTextColor", &ButtonComponent::focusedTextColor)(rttr::policy::prop::bind_as_ptr);
}

#endif