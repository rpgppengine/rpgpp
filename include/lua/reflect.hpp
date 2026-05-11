#ifndef _RPGPP_REFLECT_H
#define _RPGPP_REFLECT_H

#include <rttr/registration.h>

#include "dialogueArea.hpp"
#include "imageRect.hpp"
#include "rttr/detail/registration/registration_impl.h"
#include "uiElement.hpp"

RTTR_REGISTRATION {
	rttr::registration::class_<UIElement>("UIElement").constructor<>().property("rect", &UIElement::rect);
	rttr::registration::class_<ImageRect>("ImageRect").constructor<>().property("source", &ImageRect::source);
}

#endif