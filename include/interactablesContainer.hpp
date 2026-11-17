#ifndef _RPGPP_INTERACTABLESCONTAINER_H
#define _RPGPP_INTERACTABLESCONTAINER_H

#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

#include "baseContainer.hpp"
#include "interactable.hpp"
#include "roomtypes.hpp"
using json = nlohmann::json;

/** Container of Interactables that is to be used by a [Room](Room.md) */
class InteractablesContainer : public BaseContainer<std::unique_ptr<Interactable>> {
public:
	/** Empty constructor */
	InteractablesContainer();

	void init(std::vector<InteractableInRoomBin> &vec);
	/** Add a new Interactable with tile position and type */
	Interactable *add(IVector pos, const std::string &type);
	/** Add a new Interactable using a bin structure */
	void addBin(InteractableInRoomBin bin);
	/** Add a new Interactable using a Vector2 tile position and an interactable
	 * type in the GameBin */
	void addBinFromTypename(Vector2 pos, const std::string &type);
	/** Get an Interactable by its tile position */
	Interactable *getInt(IVector pos);
	Interactable *getIntVec2(Vector2 pos);
	/** Change the Interactable's type at the specified tile position */
	void setInteractableType(IVector pos, const std::string &type);
	/** Get the vector that contains all Interactables */
	std::vector<Interactable *> getList();
	/** Add interactables from binary structures. */
	void addBinVector(const std::vector<InteractableInRoomBin> &bin);
	/** Add interactables from a Room json object. Must contain 'interactables'
	 * and 'interactables_props' keys. */
	void addJsonData(json roomJson);
	json dumpJson();
};

#endif
