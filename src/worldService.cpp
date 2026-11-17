#include "worldService.hpp"

#include <memory>
#include <string_view>

#include "game.hpp"
#include "room.hpp"
#include "tinyfiledialogs.h"

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
	/*
	for (RoomBin bin : Game::getBin().rooms) {
		if (bin.name == roomBin) {
			deferRoomChange = true;
			deferredRoomId = bin.name;
			doFadeTransition();
			break;
		}
	}
	*/
	printf("%s \n", roomBin.c_str());
	bool res;
	auto room = Game::getResources().getUnpacker().loadT<Room>(roomBin, res);
	printf("%i \n", res);

	deferRoomChange = true;
	deferredRoomId = roomBin;
	doFadeTransition();
}

void WorldService::doFadeTransition() {
	Game::getUi().setTransitionPointCallback([this] {
		/*
		for (RoomBin bin : Game::getBin().rooms) {
			if (bin.name == deferredRoomId) {
				setRoomBin(bin);
				Game::getUi().hideInterface();
				break;
			}
		}
		*/
		printf("%s \n", deferredRoomId.c_str());
		bool res = false;
		auto room = Game::getResources().getUnpacker().loadT<Room>(deferredRoomId, res);
		if (!res) {
			tinyfd_messageBox("Error!", "Could not load the requested Room!", "ok", "error", 1);
			Game::closeGame();
		}
		this->room = std::make_unique<Room>(std::move(room));
		this->room->afterLoad();
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
