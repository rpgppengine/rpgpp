#include "lua/apiTypes.hpp"

#include <cstdio>
#include <string>

#include "actor.hpp"
#include "actorContainer.hpp"
#include "collisionsContainer.hpp"
#include "interactable.hpp"
#include "interactablesContainer.hpp"
#include "player.hpp"
#include "prop.hpp"
#include "propsContainer.hpp"
#include "raylib.h"
#include "sol/forward.hpp"
#include "sol/raii.hpp"
#include "tilemap.hpp"

void lua_interactable_setprop(Interactable *inter, sol::object key, sol::object value) {
	if (inter == nullptr) return;

	auto &props = inter->getProps();
	if (!key.is<std::string>()) {
		return;
	}

	printf("%s \n", props.dump().c_str());

	const std::string keyStr = key.as<std::string>();

	nlohmann::json &target = props[keyStr];

	if (props.count(keyStr) > 0) {
		if (props[keyStr].is_object()) {
			target = props[keyStr]["value"];
		}
	}

	if (value.is<std::string>()) {
		target = value.as<std::string>();
	} else if (value.is<int>()) {
		target = value.as<int>();
	} else if (value.is<float>()) {
		target = value.as<float>();
	} else if (value.is<bool>()) {
		target = value.as<bool>();
	}
}

void lua_types_set(sol::state_view lua) {
	lua.new_usertype<Vector2>(
		"Vector2", sol::factories([]() { return Vector2(); }, [](float a, float b) { return Vector2{a, b}; }), "x",
		&Vector2::x, "y", &Vector2::y);

	lua.new_enum("Direction", "DOWN_IDLE", Direction::RPGPP_DOWN_IDLE, "DOWN", Direction::RPGPP_DOWN, "UP_IDLE",
				 Direction::RPGPP_UP_IDLE, "UP", Direction::RPGPP_UP, "LEFT_IDLE", Direction::RPGPP_LEFT_IDLE, "LEFT",
				 Direction::RPGPP_LEFT, "RIGHT_IDLE", Direction::RPGPP_RIGHT_IDLE, "RIGHT", Direction::RPGPP_RIGHT);

	lua.new_usertype<CollisionsContainer>(sol::no_construction(), "Push", &CollisionsContainer::pushObjectVec2,
										  "Remove", &CollisionsContainer::removeObjectVec2, "Exists",
										  &CollisionsContainer::objectExistsAtPositionVec2);

	lua.new_usertype<InteractablesContainer>(
		sol::no_construction(), "Push", &InteractablesContainer::addBinFromTypename, "Remove",
		&InteractablesContainer::removeObjectVec2, "Exists", &InteractablesContainer::objectExistsAtPositionVec2,
		"GetAt", &InteractablesContainer::getIntVec2);

	lua.new_usertype<Interactable>(sol::no_construction(), "SetProp", &lua_interactable_setprop, "IsOnTouch",
								   &Interactable::isOnTouch, "SetOnTouch", &Interactable::setOnTouch, "GetPosition",
								   &Interactable::getWorldPos, "GetType", &Interactable::getType, "SetType",
								   &Interactable::setType);

	lua.new_usertype<PropsContainer>(sol::no_construction(), "Push", &PropsContainer::addProp, "Remove",
									 &PropsContainer::removeObjectVec2, "Exists",
									 &PropsContainer::objectExistsAtPositionVec2, "GetAt", &PropsContainer::getPropAt);

	lua.new_usertype<Prop>(sol::no_construction(), "GetPosition", &Prop::getWorldPos, "SetPosition", &Prop::setWorldPos,
						   "GetTilePosition", &Prop::getWorldTilePos, "SetTilePosition", &Prop::setWorldTilePos,
						   "GetInteractable", &Prop::getInteractable);

	lua.new_usertype<ActorContainer>(sol::no_construction(), "Push", &ActorContainer::addActor, "Remove",
									 &ActorContainer::removeActor, "Exists", &ActorContainer::actorExists, "Get",
									 &ActorContainer::getActor);

	lua.new_usertype<Room>(sol::no_construction(), "GetPlayer", &Room::getPlayer, "GetStartTile", &Room::getStartTile,
						   "GetTileMap", &Room::getTileMap, "GetCollisions", &Room::getCollisions, "GetInteractables",
						   &Room::getInteractables, "GetProps", &Room::getProps, "GetActors", &Room::getActors);

	lua.new_usertype<TileMap>(sol::no_construction(), "SetTile", &TileMap::setTile, "SetEmptyTile",
							  &TileMap::setEmptyTile, "GetWorldSizeInTiles", &TileMap::getMaxWorldSize);

	lua.new_usertype<Actor>(sol::no_construction(), "GetPosition", &Actor::getPosition, "SetPosition",
							&Actor::setPosition, "GetTilePosition", &Actor::getTilePosition, "SetTilePosition",
							&Actor::setTilePosition, "MoveByVelocity", &Actor::moveByVelocity, "ChangeAnimation",
							&Actor::changeAnimation, "PlayAnimation", &Actor::playAnimation);

	lua.new_usertype<Player>(sol::no_construction(), "GetPosition", &Player::getPosition, "SetPosition",
							 &Player::setPosition, "GetTilePosition", &Player::getTilePosition, "SetTilePosition",
							 &Player::setTilePosition, "MoveByVelocity", &Player::moveByVelocity, "GetActor",
							 &Player::getActor);
}

void lua_basic_types_set(sol::environment &env) {
	env.new_usertype<Vector2>(
		"Vector2", sol::factories([]() { return Vector2(); }, [](float a, float b) { return Vector2{a, b}; }), "x",
		&Vector2::x, "y", &Vector2::y);
	env.new_usertype<Rectangle>(
		"Rectangle",
		sol::factories([] { return Rectangle{}; },
					   [](float x, float y, float width, float height) { return Rectangle{x, y, width, height}; }),
		"x", &Rectangle::x, "y", &Rectangle::y, "width", &Rectangle::width, "height", &Rectangle::height,
		sol::meta_function::to_string,
		[](Rectangle rect) { return TextFormat("{%f, %f, %f, %f}", rect.x, rect.y, rect.width, rect.height); });
	env.new_usertype<Color>(
		"Color",
		sol::factories([] { return WHITE; },
					   [](unsigned char r, unsigned char g, unsigned char b) { return Color{r, g, b, 255}; }),
		"r", &Color::r, "g", &Color::g, "b", &Color::b, "a", &Color::a, sol::meta_function::to_string,
		[](Color color) { return TextFormat("{%i, %i, %i, %i}", color.r, color.g, color.b, color.a); });
	env.new_enum("NPatchLayout", "NinePatch", NPATCH_NINE_PATCH, "ThreePatchHorizontal", NPATCH_THREE_PATCH_HORIZONTAL,
				 "ThreePatchVertical", NPATCH_THREE_PATCH_VERTICAL);
	env.new_usertype<FontRef>("Font", "Path", &FontRef::path, "FontSize", &FontRef::fontSize);
	env.new_usertype<ImageRef>("Image", "Path", &ImageRef::path, "Scale", &ImageRef::scale);
	env.new_usertype<NPatchInfo>("NPatchInfo", "Top", &NPatchInfo::top, "Bottom", &NPatchInfo::bottom, "Left",
								 &NPatchInfo::left, "Right", &NPatchInfo::right);
	env.new_enum("TextAlignment", "Top", TEXT_ALIGN_BOTTOM, "Center", TEXT_ALIGN_CENTRE, "Bottom", TEXT_ALIGN_BOTTOM,
				 "Left", TEXT_ALIGN_LEFT, "Middle", TEXT_ALIGN_MIDDLE, "Right", TEXT_ALIGN_RIGHT);
	env.new_usertype<UIElementRef>("UIElementRef", "Id", &UIElementRef::entityId);
}
