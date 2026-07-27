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

	gui.setFont("resources/Silver.ttf", 21);

	auto label = std::make_shared<edui::Label>();
	label->setSize({0, 160}, {0, 26});
	//label->setText("японская транснациональная корпорация, специализирующаяся на разработке видеоигр и игровых систем.");
	label->setText("者ノト塚15車し表二ね");
	//label->setText("The lazy fox jumped over the dog!");
	label->render->as<edui::LabelRender>().padding = 2;
	label->render->as<edui::LabelRender>().fontSize = 2.0f;
	//label->render->as<edui::LabelRender>().border = 0;
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
