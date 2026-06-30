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
#include "game.hpp"
#include "gamedata.hpp"
#include "interfaceElementFactory.hpp"
#include "lua.h"
#include "lua/reflect.hpp"
#include "raylib.h"
#include "scriptService.hpp"
#include "sol/environment.hpp"
#include "sol/forward.hpp"
#include "sol/state_view.hpp"
#include "sol/types.hpp"
#include "system.hpp"
#include "tween.hpp"
#include "uiTypesApi.hpp"

InterfaceView::InterfaceView() : InterfaceView(Rectangle{}) {}

InterfaceView::InterfaceView(Rectangle rect) {
	this->rect = rect;

	ecs.init();

	ecs.registerComponent<VisibilityComponent>();
	ecs.registerComponent<Rectangle>();
	ecs.registerComponent<LabelComponent>();
	ecs.registerComponent<TextAreaComponent>();
	ecs.registerComponent<ColorRectComponent>();
	ecs.registerComponent<ImageRectComponent>();
	ecs.registerComponent<NinePatchImageRectComponent>();
	ecs.registerComponent<DialogueComponent>();
	ecs.registerComponent<ButtonComponent>();
	ecs.registerComponent<InputComponent>();

	t.a = 0;
	t.b = 5;
	t.duration = 5.0f;
	t.left = t.duration;
	t.ptr = &f;

	auto &ref = tweens.emplace_back();
	// ref.addTween({0, 5, &f, 5.0f});
}

InterfaceView::InterfaceView(const std::string &filePath) : InterfaceView(Rectangle{}) {
	std::string fileText = LoadFileText(filePath.c_str());

	auto j = json::parse(fileText);
	this->rect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

	for (auto &item : j.at("entities").items()) {
		auto obj = item.value();
		auto entity = ecs.createEntity(item.key());
		for (auto &componentJson : item.value().items()) {
			ecs.insertComponentFromJson(entity, componentJson.key(), componentJson.value());
		}
	}

	scriptSource = j.at("script");
}

InterfaceView::InterfaceView(InterfaceViewBin &bin) : InterfaceView(Rectangle{}) {
	this->bin = bin;
	ecs.init();
	ecs.registerComponent<VisibilityComponent>();
	ecs.registerComponent<Rectangle>();
	ecs.registerComponent<LabelComponent>();
	ecs.registerComponent<TextAreaComponent>();
	ecs.registerComponent<ColorRectComponent>();
	ecs.registerComponent<ImageRectComponent>();
	ecs.registerComponent<NinePatchImageRectComponent>();
	ecs.registerComponent<DialogueComponent>();
	ecs.registerComponent<ButtonComponent>();
	ecs.registerComponent<InputComponent>();

	for (auto &[title, elementCbor] : bin.entites) {
		auto entity = ecs.createEntity(title);

		auto elementJson = json::from_cbor(elementCbor);
		for (auto &componentJson : elementJson.items()) {
			ecs.insertComponentFromJson(entity, componentJson.key(), componentJson.value());
		}
		initEntityComponents(entity);
	}

	scriptSource = bin.scriptSource;

	if (!scriptSource.empty() && Game::isUsingBin()) {
		std::string key = TextFormat("scripts/%s", scriptSource.c_str());
		if (Game::getBin().scripts.count(key) > 0) {
			std::string luaCode = Game::getBin().scripts[key].bytecode;
			Game::getScripts().getState()["self"] = sol::nil;
			auto env = sol::environment(Game::getScripts().getState(), Game::getScripts().getState().globals());
			lua_ui_types_set(env);
			env["self"] = this;
			env["view"] = this;
			this->env = env;

			Game::getScripts().getState().do_string(luaCode, this->env);

			//run init function if it exists
			if (this->env["init"].is<sol::function>()) {
				this->env["init"]();
			}
		}
	}
}

nlohmann::json InterfaceView::dumpJson() {
	auto j = json::object();

	j["entities"] = json::object();
	for (auto &entity : ecs.getEntities()) {
		auto &name = ecs.getEntityName(entity);

		j["entities"][name] = json::object();

		auto set = ecs.getEntityComponents(entity);
		for (auto &componentName : set) {
			auto componentJson = ecs.getComponentJson(entity, componentName);
			j["entities"][name][componentName] = componentJson;
		}
	}

	j["script"] = scriptSource;

	return j;
}

bool InterfaceView::elementExists(const std::string &title) {
	return ecs.getEntityManager().findName(title) < MAX_ENTITIES;
}

EntityID InterfaceView::addElement(const std::string &title) { return ecs.createEntity(title); }

void InterfaceView::removeElement(const std::string &title) {
	EntityID entity = ecs.getEntityManager().findName(title);
	if (entity < MAX_ENTITIES) {
		ecs.destroyEntity(entity);
	}
}

EntityID InterfaceView::getElement(const std::string &title) { return ecs.getEntityManager().findName(title); }

const std::set<EntityID> &InterfaceView::getElements() { return ecs.getEntities(); }

void InterfaceView::renameElement(const std::string &title, const std::string &newTitle) {
	ecs.getEntityManager().renameEntity(ecs.getEntityManager().findName(title), newTitle);
}

void InterfaceView::changeFocusedElement(const std::string &title) {
	auto entity = ecs.getEntityManager().findName(title);
	if (entity <= MAX_ENTITIES) {
		changeFocusedElement(entity);
	}
}

void InterfaceView::changeFocusedElement(EntityID entity) {
	if (entity < MAX_ENTITIES) {
		if (ecs.hasComponent<InputComponent>(entity)) {
			if (current < MAX_ENTITIES) {
				auto &previousInput = ecs.getComponent<InputComponent>(current);
				if (previousInput.callbacks[CALLBACK_UNFOCUSED] != nullptr) {
					previousInput.callbacks[CALLBACK_UNFOCUSED]();
				}

				auto luaFunc = getLuaEnvironment()[previousInput.funcNames.funcNames[CALLBACK_UNFOCUSED]];
				if (luaFunc.is<sol::function>()) {
					luaFunc();
				}
			}

			auto &input = ecs.getComponent<InputComponent>(entity);
			if (input.callbacks[CALLBACK_FOCUSED] != nullptr) {
				input.callbacks[CALLBACK_FOCUSED]();
			}

			auto luaFunc = getLuaEnvironment()[input.funcNames.funcNames[CALLBACK_FOCUSED]];
			if (luaFunc.is<sol::function>()) {
				luaFunc();
			}

			current = entity;
		}
	}
}

void InterfaceView::resetElements() {
	for (auto &[title, elementCbor] : bin.entites) {
		auto entity = ecs.getEntityManager().findName(title);

		auto elementJson = json::from_cbor(elementCbor);
		for (auto &componentJson : elementJson.items()) {
			ecs.replaceComponentFromJson(entity, componentJson.key(), componentJson.value());
		}

		initEntityComponents(entity);
	}
}

void InterfaceView::onNotify(Event event) {
	if (current < MAX_ENTITIES) {
		ecs.getSystem().onNotify(event, current);
	}
}

void InterfaceView::update() {
	ecs.update();
	for (auto &container : tweens) {
		container.update();
	}
}

void InterfaceView::draw() { ecs.draw(); }

Coordinator &InterfaceView::getCoordinator() { return ecs; }

const std::set<EntityID> &InterfaceView::getEntities() { return ecs.getEntities(); }

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
		component.loadFont(component.font.path);
	}

	if (ecs.hasComponent<TextAreaComponent>(entity)) {
		auto &component = ecs.getComponent<TextAreaComponent>(entity);
		component.loadFont(component.font.path);
	}

	if (ecs.hasComponent<ImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<ImageRectComponent>(entity);
		component.scaleImage(component.image.scale);
	}

	if (ecs.hasComponent<NinePatchImageRectComponent>(entity)) {
		auto &component = ecs.getComponent<NinePatchImageRectComponent>(entity);
		component.scaleImage(component.image.scale);
	}

	if (ecs.hasComponent<ButtonComponent>(entity) && ecs.hasComponent<InputComponent>(entity)) {
		auto &input = ecs.getComponent<InputComponent>(entity);
		input.callbacks[CALLBACK_FOCUSED] = [this, entity] {
			auto &button = ecs.getComponent<ButtonComponent>(entity);
			button.shownTextColor = button.focusedTextColor;
		};
		input.callbacks[CALLBACK_UNFOCUSED] = [this, entity] {
			auto &button = ecs.getComponent<ButtonComponent>(entity);
			button.shownTextColor = button.normalTextColor;
		};
	}
}

void InterfaceView::setScriptFile(const std::string &fileName) { this->scriptSource = fileName; }

std::string InterfaceView::getScriptFile() { return scriptSource; }

sol::environment &InterfaceView::getLuaEnvironment() { return env; }

std::list<TweenContainer> &InterfaceView::getTweens() { return tweens; }

void InterfaceView::addTweenContainer(TweenContainer tweenContainer) {
	tweens.push_back(tweenContainer);
}

void InterfaceView::addTween(Tween tween) {
	auto& container = tweens.emplace_back();
	container.addTween(tween);
}
