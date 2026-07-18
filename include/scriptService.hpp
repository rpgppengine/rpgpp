#ifndef _RPGPP_SCRIPTSERVICE_H
#define _RPGPP_SCRIPTSERVICE_H

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>

#include "sol/forward.hpp"
#include "sol/state.hpp"
#include "sol/state_view.hpp"

class ScriptService {
private:
	sol::state state;

public:
	ScriptService();
	sol::state &getState();
	void setLua(sol::state_view lua);
	void addToState(nlohmann::json &j);
	sol::environment createEnvironment();
};

#endif