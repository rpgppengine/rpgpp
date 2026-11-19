#include "propsContainer.hpp"

#include <memory>

#include "conversion.hpp"
#include "game.hpp"
#include "prop.hpp"

void PropsContainer::addProp(Vector2 pos, const std::string &type) {
	/*
	for (auto const &propBin : Game::getBin().props) {
		if (propBin.name == type) {
			printf("c \n");
			auto p = std::make_unique<Prop>(propBin);
			p->setWorldTilePos(pos, Game::getWorld().getRoom().getWorldTileSize());
			p->getInteractable()->setProps(nlohmann::json::object());

			pushObject(fromVector2(pos), std::move(p));
			break;
		}
	}
	*/
}

Prop *PropsContainer::getPropAt(Vector2 pos) { return objects[fromVector2(pos)].get(); }

void PropsContainer::init(std::vector<PropInRoomBin> &vec) {
	for (auto &bin : vec) {
		bool valid = false;
		Prop prop = Game::getResources().getUnpacker().loadT<Prop>(bin.name, valid);
		prop.afterLoad();
		auto ptr = std::make_unique<Prop>(std::move(prop));
		pushObject(bin.tilePos, std::move(ptr));
	}
}
