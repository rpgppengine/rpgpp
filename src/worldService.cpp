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
	this->frameCounter = 0;
	this->transitionActive = false;
	this->transitionColor = Color{0, 0, 0, 1};
	this->alpha = 0.0f;
	this->transitionSecondStage = false;
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
	this->transitionActive = true;
	this->frameCounter = 0;
	this->transitionColor = Color{0, 0, 0, 1};
	this->alpha = 0.0f;
	this->transitionSecondStage = false;
}

Player &WorldService::getPlayer() const { return this->room->getPlayer(); }

void WorldService::update() {
	if (room == nullptr) {
		return;
	}

	if (transitionActive) {
		frameCounter++;
		if (frameCounter >= 2) {
			frameCounter = 0;
			if (!transitionSecondStage) {
				printf("%f \n", alpha);
				if (alpha < 1.0f) {
					alpha += 0.02f;
					transitionColor = Fade(transitionColor, alpha);
				} else {
					transitionSecondStage = true;
					if (deferRoomChange) {
						for (RoomBin bin : Game::getBin().rooms) {
							if (bin.name == deferredRoomId) {
								setRoomBin(bin);
								break;
							}
						}
						deferRoomChange = false;
					}
				}
			} else {
				printf("%f \n", alpha);
				if (alpha > 0.0f) {
					alpha -= 0.02f;
					transitionColor = Fade(transitionColor, alpha);
				} else {
					transitionActive = false;
					transitionSecondStage = false;
				}
			}
		}
	}

	room->update();
}

bool WorldService::getIfRoomExist() {
	return room != nullptr;
}

void WorldService::draw() const {
	if (room == nullptr) {
		return;
	}

	room->draw();

	if (transitionActive) {
		DrawRectangleRec(Rectangle{0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())},
						 transitionColor);
	}
}

void WorldService::unload() const {
	// room->unload();
}
