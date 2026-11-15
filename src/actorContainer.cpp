#include "actorContainer.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "actor.hpp"
#include "game.hpp"
#include "raylib.h"

ActorContainer::ActorContainer() { actors = std::map<std::string, std::unique_ptr<Actor>>{}; }

std::map<std::string, std::unique_ptr<Actor>> &ActorContainer::getActors() { return actors; }

Actor &ActorContainer::getActor(const std::string &name) { return *actors[name]; }

void ActorContainer::addActor(Vector2 pos, const std::string &typeName, const std::string &roomName) {
	if (Game::getBin().actors.count(typeName) > 0) {
		auto newActor = std::make_unique<Actor>(Game::getBin().actors[typeName]);
		newActor->setTilePosition(pos, Game::getWorld().getRoom().getTileMap()->getTileSet()->getTileSize());
		actors[roomName] = std::move(newActor);
	} else {
		throw std::runtime_error(TextFormat("This Actor does not exist: %s", typeName.c_str()));
	}
}

void ActorContainer::removeActor(const std::string &roomName) { actors.erase(roomName); }

bool ActorContainer::actorExists(const std::string &roomName) { return (actors.count(roomName) > 0); }
