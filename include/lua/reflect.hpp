#ifndef _RPGPP_REFLECT_H
#define _RPGPP_REFLECT_H

#include <rttr/registration.h>

#include "gamedata.hpp"
#include "raylib.h"
#include "rttr/detail/registration/registration_impl.h"
#include "rttr/policy.h"

RTTR_REGISTRATION {
	rttr::registration::class_<Rectangle>("Rectangle")
		.constructor<>()
		.property("x", &Rectangle::x)(rttr::policy::prop::bind_as_ptr)
		.property("y", &Rectangle::y)(rttr::policy::prop::bind_as_ptr)
		.property("width", &Rectangle::width)(rttr::policy::prop::bind_as_ptr)
		.property("height", &Rectangle::height)(rttr::policy::prop::bind_as_ptr);
}

#endif
