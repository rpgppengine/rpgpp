#ifndef _RPGPP_LUA_INTERFACEAPI_H
#define _RPGPP_LUA_INTERFACEAPI_H

#include <string>

#include "sol/state_view.hpp"

void lua_ui_opendiag(const std::string &id);
void lua_ui_set(sol::state_view &lua);

#endif