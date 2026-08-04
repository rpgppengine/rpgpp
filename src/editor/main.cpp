#include <memory>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/checkbox.hpp"
#include "edui/colorRect.hpp"
#include "edui/container.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/intValue.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/textBox.hpp"
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
	SetTargetFPS(60);

	gui.setFont("resources/Silver.ttf", 21);

	auto label = std::make_shared<edui::Label>();
	label->setSize({0, 160}, {0, 26});
	label->setText("者ノト塚15車し表二ね");
	label->render->as<edui::LabelRender>().padding = 2;
	label->render->as<edui::LabelRender>().fontSize = 2.0f;
	gui.add(label);

	auto textBox = std::make_shared<edui::TextBox>();
	textBox->setSize({0, 200}, {0, 26});
	textBox->setPosition({0, 20}, {0, 50});
	textBox->setText("Hello World! The lazy fox jumped over the dog!");
	textBox->render->as<edui::TextBoxRender>().fontSize = 2.0f;
	//gui.add(textBox);

	auto check = std::make_shared<edui::Checkbox>();
	check->setSize({0, 200}, {0, 26});
	check->setPosition({0, 20}, {0, 100});
	check->valueChanged = [](bool previous, bool current) {
		printf("%i, %i \n", previous, current);
	};
	gui.add(check);

	auto intval = std::make_shared<edui::IntValue>();
	intval->setSize({0, 200}, {0, 26});
	intval->setPosition({0, 20}, {0, 150});
	intval->valueChanged = [](int previous, int current) {
		printf("%i, %i \n", previous, current);
	};
	gui.add(intval);

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
