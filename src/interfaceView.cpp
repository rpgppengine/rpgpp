#include "interfaceView.hpp"

#include <algorithm>
#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "actor.hpp"
#include "apiTypes.hpp"
#include "game.hpp"
#include "gamedata.hpp"
#include "lua.hpp"
#include "lua/reflect.hpp"
#include "raylib.h"
#include "scriptService.hpp"
#include "sol/environment.hpp"
#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "sol/types.hpp"
#include "tween.hpp"
#include "uiTypesApi.hpp"

#include <cereal/types/variant.hpp>
#include <cereal/archives/json.hpp>

InterfaceView::InterfaceView() : InterfaceView(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;

	for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
		availableIds.push(i);
	}
}

InterfaceView::InterfaceView(const std::string &filePath) : InterfaceView(Rectangle{}) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

	InterfaceViewBin bin;

	{
		std::stringstream stream;
		stream << fileText;
		cereal::JSONInputArchive cerealJson(stream);
		cerealJson(bin);
	}

	this->scriptSource = bin.scriptSource;
	for (auto& [title, elementBin] : bin.elements) {
		auto element = addElement(title, elementBin.type);
		elements[element]->props = elementBin.props;
		elements[element]->config();
	}
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) : InterfaceView(Rectangle{}) {
	this->bin = bin;

	for (auto &[title, elementBin] : bin.elements) {
		auto element = addElement(title, elementBin.type);
		elements[element]->props = elementBin.props;
		elements[element]->config();
	}

	scriptSource = bin.scriptSource;

	if (!scriptSource.empty() && Game::isUsingBin()) {
		std::string key = TextFormat("scripts/%s", scriptSource.c_str());
		if (Game::getBin().scripts.count(key) > 0) {
			scriptFlag = true;

			std::string luaCode = Game::getBin().scripts[key].bytecode;

			auto env = sol::environment(Game::getScripts().getState(), Game::getScripts().getState().globals());
			lua_basic_types_set(env);
			lua_ui_types_set(env);
			this->env = env;

			env["self"] = this;
			env["view"] = this;

			Game::getScripts().getState().do_string(luaCode, this->env);

			//run init function if it exists
			if (this->env["init"].is<sol::function>()) {
				this->env["init"]();
			}
		}
	} else {
		scriptFlag = false;
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();

	InterfaceViewBin bin;
	bin.scriptSource = scriptSource;

	for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
		auto element = elements[i].get();

		if (element != nullptr) {
			UIElementBin elementBin;
			elementBin.type = element->typeName;
			elementBin.props = element->props;

			bin.elements[elementNames[i]] = elementBin;
		}
	}

	std::stringstream stream;
	{
		cereal::JSONOutputArchive cerealJson(stream);
		cerealJson(CEREAL_NVP(bin));
	}

	j = json::parse(stream.str());

	return j;
}
//
const std::string& InterfaceView::getEntityName(int index) {
	return elementNames[index];
}

ElementIndex InterfaceView::findByName(const std::string& title) {
	ElementIndex res = MAX_ELEMENTS;
	for (int i = 0; i < MAX_ELEMENTS; i++) {
		if (elementNames[i] == title) {
			res = i;
			break;
		}
	}
	return res;
}
//
bool InterfaceView::elementExists(const std::string &title) {
	return findByName(title) < MAX_ELEMENTS;
}

ElementIndex InterfaceView::addElement(const std::string& title, const std::string& type) {
	if (size >= MAX_ELEMENTS) {
		throw std::runtime_error("Too many entities");
	}

	if (findByName(title) != MAX_ELEMENTS) {
		throw std::runtime_error(TextFormat("This name is taken: %s", title.c_str()));
	}

	ElementIndex id = availableIds.front();
	availableIds.pop();

	elements[id] = std::move(InterfaceService::getFactory().constructElement(type));
	elementNames[id] = title;

	size++;

	return id;
}

void InterfaceView::removeElement(const std::string &title) {
	ElementIndex i = findByName(title);
	if (i < MAX_ELEMENTS) {
		availableIds.push(i);
		elements[i].reset();
		elementNames[i].erase();
		size--;
	}
}

UIElement* InterfaceView::getElement(const std::string& title) {
	auto i = findByName(title);
	if (i < MAX_ELEMENTS) {
		return elements[i].get();
	}
	return nullptr;
}

UIElement* InterfaceView::getElement(ElementIndex i) {
	return elements[i].get();
}

const std::array<std::unique_ptr<UIElement>, MAX_ELEMENTS>& InterfaceView::getElements() {
	return elements;
}

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {
	auto i = findByName(title);
	if (i < MAX_ELEMENTS) {
		elementNames[i] = newTitle;
	}
}

void InterfaceView::changeFocusedElement(const std::string &title) {
	auto entity = findByName(title);
	if (entity <= MAX_ELEMENTS) {
		changeFocusedElement(entity);
	}
}

void InterfaceView::changeFocusedElement(ElementIndex index) {
	if (index < MAX_ELEMENTS) {
		auto element = elements[index].get();
		if (element->props.count("input") > 0) {
			if (currentElement < MAX_ELEMENTS) {
				auto previousElement = elements[currentElement].get();
				auto previousInput = std::get<InputC>(previousElement->props["input"]);

				if (previousElement->callbacks[CALLBACK_UNFOCUSED] != nullptr) {
					previousElement->callbacks[CALLBACK_UNFOCUSED]();
				}

				if (hasScript()) {
					auto luaFunc = getLuaEnvironment()[previousInput.funcNames.funcNames[CALLBACK_UNFOCUSED]];
					if (luaFunc.is<sol::function>()) {
						luaFunc();
					}
				}
			}

			auto input = std::get<InputC>(element->props["input"]);

			if (element->callbacks[CALLBACK_FOCUSED] != nullptr) {
				element->callbacks[CALLBACK_FOCUSED]();
			}

			if (hasScript()) {
				auto luaFunc = getLuaEnvironment()[input.funcNames.funcNames[CALLBACK_FOCUSED]];
				if (luaFunc.is<sol::function>()) {
					luaFunc();
				}
			}

			currentElement = index;
		}
	}
}

UIElement* InterfaceView::cloneElement(const std::string& title, const std::string& newTitle) {
	auto ptr = getElement(title);
	if (ptr == nullptr) {
		return nullptr;
	}

	auto newIndex = addElement(newTitle, ptr->typeName);
	UIElement* newElement = getElement(newIndex);
	newElement->props = ptr->props;

	return newElement;
}

void InterfaceView::resetElements() {
	for (auto &[title, elementBin] : bin.elements) {
		auto element = findByName(title);
		elements[element]->props = elementBin.props;
		elements[element]->config();
	}
}

void InterfaceView::onNotify(Event event) {
	if (currentElement < MAX_ELEMENTS) {
		elements[currentElement]->onNotify(event);
	}
}

void InterfaceView::update() {
	for (auto &container : tweens) {
		container.update();
	}

	for (auto& element : elements) {
		if (element.get() != nullptr) {
			element->update();
		}
	}
}

void InterfaceView::draw() {
	for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
		auto element = getElement(i);
		if (element != nullptr) {
			drawEntity(i);
		}
	}
}

void InterfaceView::drawEntity(ElementIndex i) {
	auto element = elements[i].get();
	if (element == nullptr) return;

	Rectangle rect = std::get<Rectangle>(element->props["rect"]);
	bool visible = std::get<bool>(element->props["visible"]);
	if (visible) {
		element->draw(rect);
	}
}

void InterfaceView::setScriptFile(const std::string &fileName) { this->scriptSource = fileName; }

std::string InterfaceView::getScriptFile() { return scriptSource; }

bool InterfaceView::hasScript() { return scriptFlag; }

sol::environment &InterfaceView::getLuaEnvironment() { return env; }

std::list<TweenContainer> &InterfaceView::getTweens() { return tweens; }

void InterfaceView::addTweenContainer(TweenContainer tweenContainer) {
	tweens.push_back(tweenContainer);
}

void InterfaceView::addTween(Tween tween) {
	auto& container = tweens.emplace_back();
	container.addTween(tween);
}

void InterfaceView::abandonLua() {
	if (hasScript()) {
		env.abandon();
	}
}
