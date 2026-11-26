#include "widgets/filePanels/actorPanel.hpp"

#include "actor.hpp"
#include "basicTypes.hpp"
#include "raylib.h"

using namespace editor;

ActorPanel::ActorPanel() {
	// Properties
	properties = std::make_shared<PropertyList>();
	properties->translationId = "Actor.Props";
	properties->setPosition({1, -PROPERTIES_WIDTH}, {0, 0});
	properties->setSize({0, PROPERTIES_WIDTH}, {1, 0});
	add(properties);

	bindProp<Actor, TileSetRef>("TileSet", ACTOR_TILESET, &Actor::getTileSetSource, &Actor::setTileSetSource);
	bindProp<Actor, Rectangle>("Collision", ACTOR_COLLISION_RECT, &Actor::getCollisionRect, &Actor::setCollisionRect);
}

void ActorPanel::init() {
	auto &actor = resource->as<Actor>();

	properties->getProperty(ACTOR_TILESET)->setValue(actor.getTileSetSource());
	properties->getProperty(ACTOR_COLLISION_RECT)->setValue(actor.getCollisionRect());
}
