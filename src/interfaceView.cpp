#include "interfaceView.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "actor.hpp"
#include "component.hpp"
#include "coordinator.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "lua.h"
#include "raylib.h"
#include "sol/state_view.hpp"
#include "uiElement.hpp"

InterfaceView::InterfaceView() : rect(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;
	this->elements = std::multimap<int, std::unique_ptr<UIElement>, std::less<int>>{};

	ecs.init();

	ecs.registerComponent<Rectangle>();
	ecs.registerComponent<LabelComponent>();
	ecs.registerComponent<TextAreaComponent>();
	ecs.registerComponent<ColorRectComponent>();
	ecs.registerComponent<ImageRectComponent>();
	ecs.registerComponent<NinePatchImageRectComponent>();
	ecs.registerComponent<DialogueComponent>();
	ecs.registerComponent<ButtonComponent>();
	ecs.registerComponent<InputComponent>();

	/*
	auto entity = ecs.createEntity("name");
	Rectangle entityRect = {0, 0, 50, 50};
	ColorRectComponent colorRect = {GRAY};

	ecs.addComponent(entity, entityRect);
	ecs.addComponent(entity, colorRect);
	*/
}

InterfaceView::InterfaceView(const std::string &filePath) : InterfaceView(Rectangle{}) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	/*
	for (auto &item : j.at("elements").items()) {
		auto obj = item.value();
		auto element = constructElement(obj.at("type"));
		element->fromJson(obj.at("props"));
		int layer = obj.at("layer");
		addElement(item.key(), std::move(element), layer);
	}
	*/

	for (auto &item : j.at("entities").items()) {
		auto obj = item.value();
		auto entity = ecs.createEntity(item.key());
		for (auto &componentJson : item.value().items()) {
			ecs.insertComponentFromJson(entity, componentJson.key(), componentJson.value());
		}
	}
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) : InterfaceView(Rectangle{}) {
	for (auto &[title, elementBin] : bin.elements) {
		auto element = constructElement(elementBin.type);
		element->fromBin(elementBin);
		addElement(title, std::move(element), elementBin.layer);
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();
	j["elements"] = json::object();
	for (auto &&[layer, element] : elements) {
		auto obj = json::object();
		obj["type"] = element->getType();
		obj["layer"] = element->getLayer();
		obj["props"] = element->dumpJson();

		j["elements"][element->getName()] = obj;
	}

	for (auto &entity : ecs.getEntities()) {
		auto &name = ecs.getEntityName(entity);

		j["entities"][name] = json::object();

		auto set = ecs.getEntityComponents(entity);
		for (auto &componentName : set) {
			auto componentJson = ecs.getComponentJson(entity, componentName);
			j["entities"][name][componentName] = componentJson;
		}
	}

	return j;
}

bool InterfaceView::elementExists(const std::string &title) {
	bool res = false;
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			res = true;
			break;
		}
	}
	return res;
}

void InterfaceView::addElement(const std::string &title, UIElement *element, int layer) {
	element->setName(title);
	element->setLayer(layer);
	auto ptr = std::unique_ptr<UIElement>{};
	ptr.reset(element);
	this->elements.emplace(std::make_pair(layer, std::move(ptr)));
}

void InterfaceView::addElement(const std::string &title, std::unique_ptr<UIElement> element, int layer) {
	element->setName(title);
	element->setLayer(layer);
	this->elements.emplace(std::make_pair(layer, std::move(element)));
}

void InterfaceView::removeElement(const std::string &title) {
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			if (focusedElementName == title) {
				focusedElementName = "";
				focused = nullptr;
			}
			elements.erase(item.first);
			break;
		}
	}
}

UIElement *InterfaceView::getElement(const std::string &title) {
	UIElement *res = nullptr;
	for (auto &&item : elements) {
		if (item.second->getName() == title) {
			res = item.second.get();
			break;
		}
	}
	return res;
}

const std::multimap<int, std::unique_ptr<UIElement>, std::less<int>> &InterfaceView::getElements() { return elements; }

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {
	for (auto &item : elements) {
		if (item.second->getName() == title) {
			item.second->setName(newTitle);
			break;
		}
	}
}

void InterfaceView::changeFocusedElement(const std::string &title) {
	if (elementExists(title)) {
		auto *element = getElement(title);
		if (element->isFocusable()) {
			focusedElementName = title;
			if (focused != nullptr) {
				focused->invokeCallback(CALLBACK_UNFOCUSED);
			}
			focused = element;
			focused->invokeCallback(CALLBACK_FOCUSED);
		}
	}
}

void InterfaceView::onNotify(Event event) {
	if (elementExists(focusedElementName)) {
		focused = getElement(focusedElementName);
	} else {
		focused = nullptr;
	}
	if (focused != nullptr) {
		focused->onNotify(event);
	}
}

void InterfaceView::update() {
	for (auto &item : elements) {
		if (item.second->isVisible()) {
			item.second->update();
		}
	}

	ecs.update();
}

void InterfaceView::draw() {
	for (auto &item : elements) {
		if (item.second->isVisible()) {
			item.second->draw();
		}
	}

	ecs.draw();
}

Coordinator &InterfaceView::getCoordinator() { return ecs; }

const std::set<EntityID> &InterfaceView::getEntities() { return ecs.getEntities(); }

void InterfaceView::registerLua(lua_State *L) {
	sol::state_view state{L};

	for (auto &entity : getEntities()) {
		sol::table tbl = state.create_named_table(TextFormat("Entity_%i", entity));

		auto set = ecs.getEntityComponents(entity);
		for (auto &name : set) {
			tbl[name] = ecs.getLuaObject(entity, name, state.lua_state());
		}
	}

	/*
	std::string testCode = R"(
			print('hello, let me test Entity_0.Rectangle')
			print(Entity_0.Rectangle.x)
			print(Entity_0.Rectangle.y)
			print(Entity_0.Rectangle.width)
			print(Entity_0.Rectangle.height)

			print('the entity has a gray color')
			print(Entity_0.ColorRectComponent.Color.r)
			print(Entity_0.ColorRectComponent.Color.g)
			print(Entity_0.ColorRectComponent.Color.b)
			print(Entity_0.ColorRectComponent.Color.a)
		)";
		luaState.script(testCode);
		*/
}

void InterfaceView::changeFocusedElement(EntityID entity) {
	if (entity < MAX_ENTITIES) {
		if (ecs.hasComponent<InputComponent>(entity)) {
			auto &previousInput = ecs.getComponent<InputComponent>(current);
			previousInput.callbacks[CALLBACK_UNFOCUSED]();

			auto &input = ecs.getComponent<InputComponent>(entity);
			input.callbacks[CALLBACK_FOCUSED]();
			current = entity;
		}
	}
}

nlohmann::json InterfaceView::dumpEntityJson(EntityID entity) {
	nlohmann::json j = json::object();

	auto set = ecs.getEntityComponents(entity);
	for (auto &componentName : set) {
		auto componentJson = ecs.getComponentJson(entity, componentName);
		j[componentName] = componentJson;
	}

	return j;
}

void InterfaceView::initEntityComponents(EntityID entity) {
	if (ecs.hasComponent<LabelComponent>(entity)) {
		auto &component = ecs.getComponent<LabelComponent>(entity);
		std::string fullPath = TextFormat("fonts/%s", component.font.path.c_str());

		if (component.font.path.empty()) {
			auto fontPaths = LoadDirectoryFiles("fonts/");
			if (fontPaths.count > 0) {
				auto fontPath = fontPaths.paths[0];
				fullPath = fontPath;
			}
		}
		component.font.font = LoadFontEx(fullPath.c_str(), component.font.fontSize, nullptr, 256);
	}

	if (ecs.hasComponent<TextAreaComponent>(entity)) {
		auto &component = ecs.getComponent<TextAreaComponent>(entity);
		std::string fullPath = TextFormat("fonts/%s", component.font.path.c_str());
		if (component.font.path.empty()) {
			auto fontPaths = LoadDirectoryFiles("fonts/");
			if (fontPaths.count > 0) {
				auto fontPath = fontPaths.paths[0];
				fullPath = fontPath;
			}
		}
		component.font.font = LoadFontEx(fullPath.c_str(), component.font.fontSize, nullptr, 256);
	}

	if (ecs.hasComponent<ImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<ImageRectComponent>(entity);
		component.image.texture = LoadTexture(TextFormat("images/%s", component.image.path.c_str()));
	}

	if (ecs.hasComponent<NinePatchImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<NinePatchImageRectComponent>(entity);
		component.image.texture = LoadTexture(TextFormat("images/%s", component.image.path.c_str()));
	}
}
