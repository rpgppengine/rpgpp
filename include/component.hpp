#ifndef _RPGPP_COMPONENT_H
#define _RPGPP_COMPONENT_H

#include <bitset>
#include <cstdint>

#include "raylib.h"

typedef std::uint8_t ComponentType;

const ComponentType MAX_COMPONENTS = 32;

typedef std::bitset<MAX_COMPONENTS> Signature;

struct ColorRectComponent {
	Color color;
};

#endif