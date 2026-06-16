#include "interfaceElementFactory.hpp"

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "component.hpp"
#include "entity.hpp"
#include "gamedata.hpp"

static std::unordered_map<std::string, std::list<std::string>> elements = {
	{"ColorRect", {"ColorRectComponent"}},
	{"Label", {"LabelComponent"}},
	{"TextArea", {"TextAreaComponent"}},
	{"Image", {"ImageRectComponent"}},
	{"NPatchImage", {"NinePatchImageRectComponent"}},
	{"Button", {"ColorRectComponent", "LabelComponent", "InputComponent", "ButtonComponent"}},
	{"Dialogue", {"DialogueComponent"}}};

EntityID construct(const std::string &title, const std::string &element, Coordinator &ecs) {
	EntityID entity = MAX_ENTITIES;
	if (elements.count(element) > 0) {
		auto &list = elements[element];
		entity = ecs.createEntity(title);
		ecs.addComponent(entity, VisibilityComponent{});
		ecs.addComponent(entity, Rectangle{0, 0, 50, 50});
		for (auto &componentName : list) {
			ecs.insertEmptyComponent(entity, componentName);
		}

		if (ecs.hasComponent<ButtonComponent>(entity) && ecs.hasComponent<InputComponent>(entity)) {
			auto &input = ecs.getComponent<InputComponent>(entity);
			input.callbacks[CALLBACK_FOCUSED] = [&ecs, entity] {
				auto &button = ecs.getComponent<ButtonComponent>(entity);
				button.shownTextColor = button.focusedTextColor;
				printf("focused..\n");
			};
			input.callbacks[CALLBACK_UNFOCUSED] = [&ecs, entity] {
				auto &button = ecs.getComponent<ButtonComponent>(entity);
				button.shownTextColor = button.normalTextColor;
			};
		}
	}

	return entity;
}

std::list<std::string> getElementsFactoryList() {
	std::list<std::string> list;
	for (auto &pair : elements) {
		list.emplace_front(pair.first);
	}
	return list;
}