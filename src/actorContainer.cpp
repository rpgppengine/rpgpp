#include "actorContainer.hpp"

#include <memory>
#include <stdexcept>
#include <string>

#include "actor.hpp"
#include "game.hpp"
#include "raylib.h"

void ActorContainer::init(std::vector<ActorInRoomBin> &vec) {
	for (auto &bin : vec) {
		bool valid = false;
		Actor actor = Game::getResources().getUnpacker().loadT<Actor>(bin.source, valid);
		actor.afterLoad();
		// auto ptr = std::make_unique<Actor>(std::move(actor));

		ActorEntry entry;
		entry.actor = std::move(actor);
		entry.name = bin.name;
		pushObject(bin.tilePos, std::move(entry));
	}
}

Actor &ActorContainer::getActor(const std::string &name) {
	for (auto &[key, entry] : objects) {
		if (entry.name == name) {
			return entry.actor;
		}
	}
	return objects.begin()->second.actor;
}

void ActorContainer::addActor(Vector2 pos, const std::string &typeName, const std::string &roomName) {
	/*
	if (Game::getBin().actors.count(typeName) > 0) {
		auto newActor = std::make_unique<Actor>(Game::getBin().actors[typeName]);
		newActor->setTilePosition(pos, Game::getWorld().getRoom().getTileMap()->getTileSet()->getTileSize());
		actors[roomName] = std::move(newActor);
	} else {
		throw std::runtime_error(TextFormat("This Actor does not exist: %s", typeName.c_str()));
	}
	*/
}

void ActorContainer::removeActor(const std::string &name) {
	for (auto it = objects.begin(); it != objects.end();) {
		if (it->second.name == name) {
			it = objects.erase(it);
			return;
		} else {
			it++;
		}
	}
}

bool ActorContainer::actorExists(const std::string &name) {
	for (auto &[key, entry] : objects) {
		if (entry.name == name) {
			return true;
		}
	}
	return false;
}
