#include "editor.hpp"

#include <memory>

#include "raylib.h"
#include "services/interface.hpp"
#include "services/resources.hpp"

using namespace editor;

Editor *Editor::instance = nullptr;

Editor::Editor() {
	Editor::instance = this;

	initWindow();

	resources = std::make_unique<Resources>();
	interface = std::make_unique<Interface>();
}

Editor::~Editor() {}

void Editor::initWindow() {
	InitWindow(EDITOR_WINDOW_WIDTH, EDITOR_WINDOW_HEIGHT, "Editor");

	Image iconImage = LoadImage("resources/app-icon.png");
	SetWindowIcon(iconImage);
	UnloadImage(iconImage);

	SetTargetFPS(EDITOR_FPS);
}

void Editor::run() {
	while (!WindowShouldClose()) {
		update();

		BeginDrawing();
		draw();
		EndDrawing();
	}

	unload();
	CloseWindow();
}

void Editor::update() {
	resources->update();
	interface->update();
}

void Editor::draw() { interface->draw(); }

void Editor::unload() { interface->unload(); }

Interface &Editor::getInterface() { return *interface; }

Resources &Editor::getResources() { return *resources; }

Editor &Editor::getInstance() { return *Editor::instance; }
