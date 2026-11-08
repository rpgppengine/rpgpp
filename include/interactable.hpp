#ifndef _RPGPP_INTERACTABLE_H
#define _RPGPP_INTERACTABLE_H

#include <raylib.h>

#include <array>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "gamedata.hpp"
#include "resource.hpp"
#include "saveable.hpp"

using json = nlohmann::json;
enum PropType { PROP_INT, PROP_STRING, PROP_BOOLEAN, PROP_DIALOGUE };

/** Defines an object that is interactable in-game by a player's action */
class Interactable : public Resource {
private:
	/** Whether this Interactable is valid */
	bool valid;
	/** The type of this Interactable */
	std::string type;
	/** The displayed title in the Editor. */
	std::string displayTitle;
	/** The position of this Interactable in terms of tiles */
	Vector2 tilePos;
	/** Tile size in pixels */
	int tileSize;
	/** The absolute position of this Interactable in the world scene */
	Vector2 absolutePos;
	/** The Rectangle of this Interactable */
	Rectangle rect;
	/** Whether this interactable will be interacted with on touch. */
	bool onTouch;
	/** A json object describing the properties of an interactable. */
	std::unique_ptr<nlohmann::json> props;
	/** Script file path */
	std::string scriptPath;

public:
	/** Empty constructor */
	Interactable();
	/** Constructor using a JSON file. */
	Interactable(const std::string &path);
	/** Construct from type name */
	Interactable(const std::string &type, Vector2 tilePos, int tileSize);
	/** Construct from a bin */
	Interactable(InteractableInRoomBin bin);
	virtual ~Interactable() = default;
	/** Dump JSON data. */
	// nlohmann::json dumpJson();
	/** Whether this Interactable is valid */
	bool isValid() const;
	/** Get the Rectangle of this Interactable */
	Rectangle getRect() const;
	/** Get this Interactable's type. */
	const std::string &getType() const;
	/** Get the World position of this Interactable. */
	Vector2 getWorldPos() const;
	/** Get the onTouch member. */
	bool isOnTouch() const;
	/** Set whether this interactable will be invoked when player touches it. */
	void setOnTouch(bool onTouch);
	/** Invoke the interact method. Each interactable has a script with an
	 * 'interact' function. */
	void interact();
	/** Change the interactable's type .*/
	void setType(const std::string &type);
	/** Set a property of this interactable. */
	void setProp(std::string key, std::string value);
	/** Set properties using a nlohmann::json object. */
	void setProps(nlohmann::json j);
	/** Add a property based on the chosen property type. */
	void addProp(PropType propType, const std::string &name);
	/** Set the source script file path. */
	void setScriptSourcePath(const std::string &newPath);
	/** Get source script file. */
	const std::string &getScriptSourcePath();
	/** Get the properties json object. */
	nlohmann::json &getProps();
	/** Get a pointer to the properties. */
	nlohmann::json *getPropsPtr();
	/** Set the Interactable's display title (shown in Editor) */
	void setDisplayTitle(const std::string &newTitle);
	/** Get the Interactable's display title. */
	std::string &getDisplayTitle();

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(displayTitle), CEREAL_NVP(scriptPath), CEREAL_NVP(onTouch),
		  cereal::make_nvp("props", props->dump()));
	}

	void loadFile(std::filesystem::path filePath) override {
		char *jsonString = LoadFileText(filePath.c_str());
		nlohmann::json intJson = json::parse(jsonString);
		UnloadFileText(jsonString);

		type = GetFileNameWithoutExt(filePath.c_str());
		displayTitle = intJson.at("name");
		props = std::make_unique<nlohmann::json>(intJson.at("props"));
		scriptPath = intJson.at("script");
		onTouch = intJson.at("onTouch");
	}

	void saveFile(std::filesystem::path filePath) override {}
};

#include <cereal/types/polymorphic.hpp>
CEREAL_REGISTER_TYPE(Interactable)
CEREAL_REGISTER_POLYMORPHIC_RELATION(Resource, Interactable)

#endif
