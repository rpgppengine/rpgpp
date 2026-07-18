#include "interactable.hpp"

#include <raylib.h>
#include <stdio.h>

#include <exception>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>

#include "actor.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceService.hpp"
#include "sol/forward.hpp"
#include "sol/state.hpp"
#include "sol/state_handling.hpp"
#include "sol/types.hpp"
#include "tilemap.hpp"

Interactable::Interactable() : type(), tilePos(), tileSize(0), absolutePos(), rect() {
	this->valid = false;
	this->onTouch = false;
}

Interactable::Interactable(const std::string &path) {
	char *jsonString = LoadFileText(path.c_str());
	json intJson = json::parse(jsonString);
	UnloadFileText(jsonString);

	type = GetFileNameWithoutExt(path.c_str());
	displayTitle = intJson.at("name");
	props = std::make_unique<nlohmann::json>(intJson.at("props"));
	scriptPath = intJson.at("script");
	onTouch = intJson.at("onTouch");
}

Interactable::Interactable(const std::string &type, Vector2 tilePos, int tileSize) {
	this->type = type;
	this->props = std::make_unique<nlohmann::json>(json::object());

	this->valid = true;
	this->onTouch = false;
	this->type = type;
	this->tilePos = tilePos;
	this->tileSize = tileSize;
	this->absolutePos = Vector2{0, 0};

	rect = Rectangle{tilePos.x * tileSize, tilePos.y * tileSize, static_cast<float>(tileSize),
					 static_cast<float>(tileSize)};
}

Interactable::Interactable(InteractableInRoomBin bin) {
	this->type = bin.type;
	this->props = std::make_unique<nlohmann::json>(json::from_cbor(bin.propsCbor));

	Vector2 tilePos = {static_cast<float>(bin.x), static_cast<float>(bin.y)};

	this->valid = true;
	this->onTouch = bin.onTouch;
	this->tilePos = tilePos;
	this->tileSize = _RPGPP_TILESIZE;
	this->absolutePos = Vector2{0, 0};

	rect = Rectangle{tilePos.x * tileSize, tilePos.y * tileSize, static_cast<float>(tileSize),
					 static_cast<float>(tileSize)};
}

json Interactable::dumpJson() {
	json j = json::object();
	j.push_back({"name", displayTitle});
	j.push_back({"props", *props});
	j.push_back({"onTouch", onTouch});
	j.push_back({"script", scriptPath});

	return j;
}

bool Interactable::isValid() const { return this->valid; }

Rectangle Interactable::getRect() const { return this->rect; }

Vector2 Interactable::getWorldPos() const { return this->tilePos; }

bool Interactable::isOnTouch() const { return onTouch; }

void Interactable::setOnTouch(bool onTouch) { this->onTouch = onTouch; }

const std::string &Interactable::getType() const { return this->type; }

void Interactable::setType(const std::string &type) {
	this->type = type;
	this->props = std::make_unique<nlohmann::json>(json::object());
}

void Interactable::setProps(nlohmann::json j) { this->props = std::make_unique<nlohmann::json>(j); }

void Interactable::addProp(PropType propType, const std::string &name) {
	switch (propType) {
		case PROP_INT:
			props->push_back({name, 0});
			break;
		case PROP_STRING:
			props->push_back({name, ""});
			break;
		case PROP_BOOLEAN:
			props->push_back({name, false});
			break;
		case PROP_DIALOGUE:
			props->push_back({name, {{"propType", "dialogue"}, {"value", ""}}});
			break;
		default:
			break;
	}
}

nlohmann::json &Interactable::getProps() { return *props; }

nlohmann::json *Interactable::getPropsPtr() { return props.get(); }

void Interactable::setScriptSourcePath(const std::string &newPath) { scriptPath = newPath; }

const std::string &Interactable::getScriptSourcePath() { return scriptPath; }

void Interactable::setDisplayTitle(const std::string &newTitle) { displayTitle = newTitle; }

std::string &Interactable::getDisplayTitle() { return displayTitle; }

void Interactable::interact() {
	if (type.empty()) {
		printf("%s \n", "Warning: This Interactable's type is empty.");
		return;
	}

	auto &state = Game::getScripts().getState();

	Game::getScripts().addToState(*props);
	state["this"] = this;
	state["self"] = this;

	auto intBin = Game::getBin().interactables.at(type);
	if (Game::getBin().scripts.count(intBin.scriptPath) != 0) {
		auto bc = Game::getBin().scripts[intBin.scriptPath].bytecode;
		auto result = state.safe_script(bc, &sol::script_pass_on_error);

		if (!result.valid()) {
			sol::error error = result;
			std::cout << error.what() << std::endl;
		}
		if (result.status() != sol::call_status::ok) {
			printf("uh oh: %i \n", static_cast<int>(result.status()));
		}

		if (state["interact"].valid()) {
			sol::protected_function f(state["interact"]);
			auto func_result = f();
			if (!func_result.valid()) {
				sol::error error = func_result;
				std::cout << error.what() << std::endl;
			}
		}
	}
}
