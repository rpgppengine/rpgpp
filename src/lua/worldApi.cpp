#include "lua/worldApi.hpp"

#include "game.hpp"
#include "sol/forward.hpp"
#include "sol/object.hpp"
#include "sol/table.hpp"
#include "sol/types.hpp"

sol::object lua_world_getroom(sol::this_state lua) { return sol::make_object(lua, &Game::getWorld().getRoom()); }

void lua_world_setroom(const std::string &room) { Game::getWorld().setRoomBin(room); }

sol::object lua_world_getplayer(sol::this_state lua) { return sol::make_object(lua, &Game::getWorld().getPlayer()); }

void lua_world_set(sol::state_view lua) {
	auto space = lua["World"].get_or_create<sol::table>();

	space.set_function("GetRoom", lua_world_getroom);
	space.set_function<void(const std::string &)>("SetRoom", lua_world_setroom);
	space.set_function("GetPlayer", lua_world_getplayer);

	auto gameSpace = lua["Game"].get_or_create<sol::table>();
	gameSpace.set_function("Close", &Game::closeGame);
}
