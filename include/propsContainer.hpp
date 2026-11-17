#ifndef _RPGPP_PROPSCONTAINER_H
#define _RPGPP_PROPSCONTAINER_H

#include <memory>
#include <vector>

#include "baseContainer.hpp"
#include "prop.hpp"
#include "roomtypes.hpp"

class PropsContainer : public BaseContainer<std::unique_ptr<Prop>> {
public:
	PropsContainer() = default;
	void addProp(Vector2 pos, const std::string &type);
	Prop *getPropAt(Vector2 pos);

	void init(std::vector<PropInRoomBin> &vec);
};

#endif
