#include "interfaceElementFactory.hpp"

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "button.hpp"
#include "colorRect.hpp"
#include "component.hpp"
#include "dialogueArea.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "imageRect.hpp"
#include "label.hpp"
#include "ninePatchImageRect.hpp"
#include "textArea.hpp"
#include "uiElement.hpp"

static std::array<std::function<std::unique_ptr<UIElement>()>, RPGPP_INTERFACE_ELEMENT_MAX> interfaceFactory = {
	[] { return std::make_unique<UIElement>(); }, [] { return std::make_unique<TextArea>(); },
	[] { return std::make_unique<Label>(); },		[] { return std::make_unique<ColorRect>(); },
	[] { return std::make_unique<ImageRect>(); }, [] { return std::make_unique<NinePatchImageRect>(); },
	[] { return std::make_unique<Button>(); },	   [] { return std::make_unique<DialogueArea>(); }};

std::unique_ptr<UIElement> constructElement(InterfaceElementType type) { return interfaceFactory[type](); }

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