#ifndef _RPGPP_ROOM_H
#define _RPGPP_ROOM_H

#include "baseContainer.hpp"
#include "propsContainer.hpp"
#include "resource.hpp"
#include "roomtypes.hpp"
constexpr const char *DEFAULT_PLAYER_PATH = "actors/playerActor.ractor";

class Player;
class TileMap;

#include <raylib.h>

#include <memory>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "gamedata.hpp"

using json = nlohmann::json;
#include "actor.hpp"
#include "actorContainer.hpp"
#include "collisionsContainer.hpp"
#include "interactablesContainer.hpp"
#include "player.hpp"
#include "prop.hpp"
#include "tilemap.hpp"

/**
 * This class represents a game's room. It can contain the Player,
 * interactables, collisions and the TileMap
 */
class Room : public Resource {
private:
	bool lock;
	int worldTileSize;
	/** The camera. */
	Camera2D camera{};
	/** Start point of the player. (tile position) */
	Vector2 startTile{};
	/** Source of the background music for this room. */
	std::string musicSource;
	/** Container of the interactable tiles */
	std::unique_ptr<InteractablesContainer> interactables;
	/** Container of the collision tiles */
	std::unique_ptr<CollisionsContainer> collisions;
	/** Container of the props */
	std::unique_ptr<PropsContainer> props;
	/** This Room's TileMap, which contains all placed tiles. */
	std::unique_ptr<TileMap> tileMap;
	/** A collection of all Actors in this Room */
	std::unique_ptr<ActorContainer> actors;
	/** This Room's only Player. */
	std::unique_ptr<Player> player;
	void updateCamera();

	RoomData binData;
	friend class cereal::access;

public:
	/** Empty constructor */
	Room();
	/** Construct a Room from an .rmap file */
	Room(const std::string &fileName, int tileSize = 48, bool createPlayer = true);
	/** Construct a Room by using a TileMap pointer */
	Room(std::unique_ptr<TileMap> tileMap);
	/** Construct a Room by using the RoomBin binary structure
	 *@param bin The binary structure to use */
	Room(const RoomBin &bin);
	/** Dump the room's data into a nlohmann::json object for later use
	 * Useful for dumping into a .json file.
	 * @returns The JSON object */
	// json dumpJson();
	/** Unload used resources. Typically unloads used Textures by calling
	 * raylib's UnloadTexture */
	void unload() const;
	/** Update routine for this Room. */
	void update();
	/** Draw routine for this Room. */
	void draw() const;
	void setWorldTileSize(int tileSize);
	int getWorldTileSize() const;
	void setLock(bool val);
	/** Add the Player into this Room. A unique_ptr is passed whose ownership
	 * will be moved into the Room.
	 * @param newPlayer A pointer to the player that will be moved into this
	 * Room.
	 */
	void addPlayer(std::unique_ptr<Player> newPlayer);
	/** Get a reference to the Player object.
	 * @returns The Player in this Room.
	 */
	Player &getPlayer() const;
	/** Get a pointer to this Room's TileMap.
	 * @returns A pointer to the room's TileMap.
	 */
	TileMap *getTileMap() const;
	void setTileMap(TileMap *newTileMap);

	/** Get filename of the source music. */
	std::string getMusicSource() const;
	/** Set music source. */
	void setMusicSource(const std::string_view &newMusicSource);
	/** Get the start tile position. */
	Vector2 getStartTile() const;
	/** Set the start tile position. */
	void setStartTile(Vector2 newStartTile);

	/** Get a reference to the CollisionsContainer of this Room. */
	CollisionsContainer &getCollisions() const;
	/** Get a reference to the InteractablesContainer of this Room. */
	InteractablesContainer &getInteractables() const;
	/** Get a reference to the PropsContainer of this Room. */
	PropsContainer &getProps() const;
	/** Get a refernece to the collection of Actors. */
	ActorContainer &getActors();

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(startTile), CEREAL_NVP(musicSource), cereal::make_nvp("collisions", binData.collisions),
		  cereal::make_nvp("interactables", binData.interactables), cereal::make_nvp("props", binData.props),
		  cereal::make_nvp("tileMap", binData.tiles));
	}

	void loadFile(std::filesystem::path filePath) override {
		this->lock = false;

		Camera2D initialCamera;
		initialCamera.offset = Vector2{0, 0};
		initialCamera.target = Vector2{0, 0};
		initialCamera.rotation = 0.0f;
		initialCamera.zoom = 1.0f;
		this->camera = initialCamera;

		this->musicSource = "";
		this->worldTileSize = 16;

		char *jsonString = LoadFileText(filePath.c_str());
		json roomJson = json::parse(jsonString);
		UnloadFileText(jsonString);

		std::vector<int> startPosVec = roomJson.at("start_pos");
		startTile = Vector2{static_cast<float>(startPosVec[0]), static_cast<float>(startPosVec[1])};

		this->interactables = std::make_unique<InteractablesContainer>();
		this->collisions = std::make_unique<CollisionsContainer>();
		this->actors = std::make_unique<ActorContainer>();
		this->props = std::make_unique<PropsContainer>();
		this->tileMap = std::make_unique<TileMap>(filePath);

		bool createPlayer = false;
		if (createPlayer) {
			auto actor = std::make_unique<Actor>(DEFAULT_PLAYER_PATH);
			actor->setTilePosition(Vector2{startTile.x, startTile.y}, tileMap->getTileSet()->getTileSize());
			auto initialPlayer = std::make_unique<Player>(std::move(actor), *this);

			this->addPlayer(std::move(initialPlayer));
		}

		std::vector<std::vector<int>> collisionsVec = roomJson.at("collision");
		for (auto v : collisionsVec) {
			int x = v[0];
			int y = v[1];

			collisions->pushObject({x, y}, false);
		}

		std::map<std::string, nlohmann::basic_json<>> propsVec = roomJson.at("props");
		for (auto const &[key, value] : propsVec) {
			int count = 0;
			char **textSplit = TextSplit(key.c_str(), ';', &count);
			if (count != 2) return;
			int x = std::stoi(std::string(textSplit[0]));
			int y = std::stoi(std::string(textSplit[1]));

			auto propVec = value;
			auto p = std::make_unique<Prop>(propVec.at("src"));
			p->setWorldTilePos(Vector2{static_cast<float>(x), static_cast<float>(y)}, worldTileSize);

			p->getInteractable()->setProps(propVec.at("props"));

			props->pushObject({x, y}, std::move(p));
		}

		std::map<std::string, nlohmann::json> actorsVec = roomJson.at("actors");
		for (auto const &[key, value] : actorsVec) {
			int count = 0;
			char **textSplit = TextSplit(key.c_str(), ';', &count);
			if (count != 2) return;
			int x = std::stoi(std::string(textSplit[0]));
			int y = std::stoi(std::string(textSplit[1]));

			auto a = std::make_unique<Actor>(value.at("source"));
			a->setTilePosition(Vector2{static_cast<float>(x), static_cast<float>(y)},
							   tileMap->getTileSet()->getTileSize());

			std::string intType = value.at("intType");

			if (intType.empty()) {
				a->setHasInteractable(false);
			} else {
				a->setHasInteractable(true);
				a->getInteractable()->setType(intType);
				a->getInteractable()->setProps(value.at("props"));
			}

			actors->getActors()[value.at("name")] = std::move(a);
		}

		interactables->addJsonData(roomJson.at("interactables"));

		musicSource = roomJson.at("music_source");
	}

	void saveFile(std::filesystem::path filePath) override {}
};

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(Room)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Room)

#endif
