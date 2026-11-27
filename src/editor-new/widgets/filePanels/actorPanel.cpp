#include "widgets/filePanels/actorPanel.hpp"

#include <memory>

#include "actor.hpp"
#include "basicTypes.hpp"
#include "raylib.h"
#include "services/interface.hpp"
#include "widgets/values/valuewidget.hpp"
#include "widgets/worldView/actorCanvas.hpp"

using namespace editor;

ActorPanel::ActorPanel() {
	// Properties
	properties = std::make_shared<PropertyList>();
	properties->translationId = "Actor.Props";
	properties->setPosition({1, -PROPERTIES_WIDTH}, {0, 0});
	properties->setSize({0, PROPERTIES_WIDTH}, {1, 0});
	add(properties);

	bindProp<Actor, TileSetRef>("TileSet", ACTOR_TILESET, &Actor::getTileSetSource, &Actor::setTileSetSource);
	auto info = bindProp<Actor, Rectangle>("Collision", ACTOR_COLLISION_RECT, &Actor::getCollisionRect,
										   &Actor::setCollisionRect);

	canvas = std::make_shared<ActorCanvas>();
	canvas->setSize({1, -(PROPERTIES_WIDTH + INTERFACE_SPACING)}, {1, 0});
	add(canvas);

	canvas->onCollisionChanged.connect([this, info](Rectangle a, Rectangle b) {
		info.widget.as<ValueWidgetT<Rectangle>>().onValueChanged.invoke(a, b);
	});
}

void ActorPanel::init() {
	auto &actor = resource->as<Actor>();

	properties->getProperty(ACTOR_TILESET)->setValue(actor.getTileSetSource());
	properties->getProperty(ACTOR_COLLISION_RECT)->setValue(actor.getCollisionRect());

	auto ptr = &actor;
	canvas->setActor(ptr);
}
