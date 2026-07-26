#include <memory>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/colorRect.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/label.hpp"
#include "edui/verticalContainer.hpp"
#include "raylib.h"
#include "services/editorGuiService.hpp"

#define SOL_EXCEPTIONS_SAFE_PROPAGATION

int main() {
	/*
	const auto editor = std::make_unique<Editor>();
	auto &gui = editor->getGui();

	gui.init();
	gui.uiLoop();
	editor->unload();
	*/

	edui::Gui gui;

	InitWindow(800, 450, "raylib example - basic window");

	int codepoints[512 + 143 + 68] = { 0 };
	for (int i = 0; i < 95; i++) codepoints[i] = 32 + i;
	for (int i = 0; i < 255; i++) codepoints[96 + i] = 0x400 + i; // cyrillic
	for (int i = 0; i < 143; i++) codepoints[256 + 96 + i] = 0x370 + i; // greek
	for (int i = 0; i < 68; i++) codepoints[256 + 96 + 144 + i] = 0x5B0 + i; // hebrew
	gui.font = LoadFontEx("resources/PixelCode.ttf", 12, codepoints, 512 + 143 + 68);

	auto label = std::make_shared<edui::Label>();
	label->setSize({0, 160}, {0, 26});
	label->setText("Нека говорим на езици!");
	label->render->as<edui::LabelRender>().padding = 2;
	label->render->as<edui::LabelRender>().fontSize = 2;
	label->render->as<edui::LabelRender>().border = 0;
	gui.add(label);

	while (!WindowShouldClose()) {
		gui.update();

		BeginDrawing();

		gui.draw();

		EndDrawing();
	}

	gui.unload();

	CloseWindow();

	printf("Closing editor..\n");

	return 0;
}
