#include "interfaceView.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "actor.hpp"
#include "component.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "raylib.h"
#include "uiElement.hpp"

InterfaceView::InterfaceView() : rect(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;
	this->elements = std::multimap<int, std::unique_ptr<UIElement>, std::less<int>>{};

	ecs.init();

	ecs.registerComponent<Rectangle>();
	ecs.registerComponent<ColorRectComponent>();

	auto entity = ecs.createEntity();
	Rectangle entityRect = {0, 0, 50, 50};
	ColorRectComponent colorRect = {GRAY};

	ecs.addComponent(entity, entityRect);
	ecs.addComponent(entity, colorRect);
}

InterfaceView::InterfaceView(const std::string &filePath) : InterfaceView(Rectangle{}) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
	for (auto &item : j.at("elements").items()) {
		auto obj = item.value();
		auto element = constructElement(obj.at("type"));
		element->fromJson(obj.at("props"));
		int layer = obj.at("layer");
		addElement(item.key(), std::move(element), layer);
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
}

void InterfaceView::draw() {
	for (auto &item : elements) {
		if (item.second->isVisible()) {
			item.second->draw();
		}
	}

	ecs.draw();
}
