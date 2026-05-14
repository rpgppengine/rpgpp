#include "system.hpp"

#include <algorithm>

#include "component.hpp"
#include "raylib.h"

void System::draw() {
	if (components == nullptr || entitiesManager == nullptr) return;

	for (auto &entity : entities) {
		auto rect = components->getComponent<Rectangle>(entity);

		if (hasComponent<ColorRectComponent>(entity)) {
			auto colorRect = components->getComponent<ColorRectComponent>(entity);

			DrawRectangleRec(rect, colorRect.color);
		}
	}
}