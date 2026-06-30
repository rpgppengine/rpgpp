#include "lua/interfaceApi.hpp"

#include "game.hpp"
#include "sol/table.hpp"

void lua_opendiag(const std::string &id) { Game::getUi().showDialogue(id); }

InterfaceView* lua_getui() { return Game::getUi().getCurrentView(); }

void lua_ui_set(sol::state_view &lua) {
	auto space = lua["Interface"].get_or_create<sol::table>();
	space.set_function("OpenDialogue", lua_opendiag);
	space.set_function("GetCurrentView", lua_getui);
}
