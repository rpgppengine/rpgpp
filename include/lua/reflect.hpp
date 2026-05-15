#ifndef _RPGPP_REFLECT_H
#define _RPGPP_REFLECT_H

#include <rttr/registration.h>

#include "component.hpp"
#include "dialogueArea.hpp"
#include "imageRect.hpp"
#include "raylib.h"
#include "rttr/detail/registration/registration_impl.h"
#include "uiElement.hpp"

RTTR_REGISTRATION {
	rttr::registration::class_<UIElement>("UIElement").constructor<>().property("rect", &UIElement::rect);
	rttr::registration::class_<ImageRect>("ImageRect").constructor<>().property("source", &ImageRect::source);

	rttr::registration::class_<Rectangle>("Rectangle").constructor<>();
	rttr::registration::class_<ColorRectComponent>("ColorRectComponent").constructor<>();
}

#endif