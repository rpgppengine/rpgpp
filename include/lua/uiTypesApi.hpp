#ifndef _RPGPP_LUA_UITYPESAPI_H
#define _RPGPP_LUA_UITYPESAPI_H

#include "entity.hpp"
#include "sol/forward.hpp"
#include "sol/state_view.hpp"

struct EntityWrapper {
	EntityID entity;
};

void lua_ui_types_set(sol::environment &env);

#endif
