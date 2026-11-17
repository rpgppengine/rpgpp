#ifndef _RPGPP_UITYPES_H
#define _RPGPP_UITYPES_H

#include <string>
#include <unordered_map>

#include "basicTypes.hpp"

struct UIElementBin {
	std::string type = "";
	std::unordered_map<std::string, ElementProperty> props;

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(type), CEREAL_NVP(props));
	}
};

struct InterfaceViewBin {
	std::string scriptSource = "";
	std::unordered_map<std::string, std::vector<std::uint8_t>> entites;
	std::unordered_map<std::string, UIElementBin> elements = {};

	template <typename Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(scriptSource), CEREAL_NVP(entites), CEREAL_NVP(elements));
	}
};

#endif
