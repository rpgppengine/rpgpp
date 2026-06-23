#include "scriptService.hpp"

#include <lua.hpp>
#include <nlohmann/json_fwd.hpp>

#include "lua/apiTypes.hpp"
#include "lua/interfaceApi.hpp"
#include "lua/soundsApi.hpp"
#include "lua/stateApi.hpp"
#include "lua/worldApi.hpp"
#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "sol/table.hpp"

static int wrap_exceptions(lua_State *L, lua_CFunction f) {
	try {
		return f(L);		   // Call wrapped function and return result.
	} catch (const char *s) {  // Catch and convert exceptions.
		lua_pushstring(L, s);
	} catch (std::exception &e) {
		lua_pushstring(L, e.what());
	} catch (...) {
		lua_pushliteral(L, "caught (...)");
	}
	return lua_error(L);  // Rethrow as a Lua error.
}

ScriptService::ScriptService() {
	state.open_libraries(sol::lib::base, sol::lib::string, sol::lib::os, sol::lib::table);
	lua_pushlightuserdata(state.lua_state(), (void *)wrap_exceptions);
	luaJIT_setmode(state.lua_state(), -1, LUAJIT_MODE_WRAPCFUNC | LUAJIT_MODE_ON);
	lua_pop(state.lua_state(), 1);
	setLua(state);
}

void ScriptService::setLua(sol::state_view lua) {
	lua_types_set(lua);
	lua_ui_set(lua);
	lua_sounds_set(lua);
	lua_gamestate_set(lua);
	lua_world_set(lua);
}

sol::state &ScriptService::getState() { return state; }

void ScriptService::addToState(nlohmann::json &j) {
	auto props = state["props"].get_or_create<sol::table>();
	props.clear();

	for (auto prop : j.items()) {
		if (prop.value().is_object()) {
			props[prop.key()] = prop.value().at("value").get<std::string>();
		} else if (prop.value().is_string()) {
			props[prop.key()] = prop.value().get<std::string>();
		} else if (prop.value().is_number()) {
			props[prop.key()] = prop.value().get<float>();
		}
	}
}

sol::environment ScriptService::createEnvironment() {
	sol::environment env(state);
	return env;
}