#ifndef _RPGPP_LUA_SOUNDSAPI_H
#define _RPGPP_LUA_SOUNDSAPI_H

#include <string>

#include "sol/state_view.hpp"

void lua_sounds_loadMusic(const std::string &id);
void lua_sounds_playMusic();
void lua_sounds_playSound(const std::string &id);
void lua_sounds_set(sol::state_view &lua);

#endif