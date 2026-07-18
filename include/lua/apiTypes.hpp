#ifndef _RPGPP_LUA_APITYPES_H
#define _RPGPP_LUA_APITYPES_H

#include "sol/state_view.hpp"
struct lua_Vector2 {
	float x, y;

	lua_Vector2() {
		x = 0;
		y = 0;
	}

	lua_Vector2(float x, float y) {
		this->x = x;
		this->y = y;
	}
};

void lua_types_set(sol::state_view lua);
void lua_basic_types_set(sol::environment &env);

#endif
