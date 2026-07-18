#include "worldService.hpp"

#include <memory>
#include <string_view>

#include "game.hpp"
#include "room.hpp"

WorldService::WorldService() {
	this->lock = false;
	this->room = std::unique_ptr<Room>{};
	this->deferRoomChange = false;
	this->deferredRoomId = "";
}

Room &WorldService::getRoom() const { return *this->room; }

void WorldService::setRoom(const std::string_view &filePath) {
	this->room = std::make_unique<Room>(std::string(filePath));
}

void WorldService::setRoomBin(RoomBin bin) { this->room = std::make_unique<Room>(bin); }

void WorldService::setRoomBin(const std::string &roomBin) {
	for (RoomBin bin : Game::getBin().rooms) {
		if (bin.name == roomBin) {
			deferRoomChange = true;
			deferredRoomId = bin.name;
			doFadeTransition();
			break;
		}
	}
}

void WorldService::doFadeTransition() {
	Game::getUi().setTransitionPointCallback([this] {
		for (RoomBin bin : Game::getBin().rooms) {
			if (bin.name == deferredRoomId) {
				setRoomBin(bin);
				Game::getUi().hideInterface();
				break;
			}
		}
	});
	Game::getUi().doFadeTransition();
}

Player &WorldService::getPlayer() const { return this->room->getPlayer(); }

void WorldService::update() {
	if (room != nullptr) {
		room->update();
	}
}

bool WorldService::getIfRoomExist() { return room != nullptr; }

void WorldService::draw() const {
	if (room != nullptr) {
		room->draw();
	}
}

void WorldService::unload() const {
	// room->unload();
}
