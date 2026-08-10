#include <memory>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/checkbox.hpp"
#include "edui/colorRect.hpp"
#include "edui/container.hpp"
#include "edui/dropdown.hpp"
#include "edui/dropdownList.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/intValue.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/dropdown.hpp"
#include "raylib.h"
#include "services/editorGuiService.hpp"

#include "edui/rlicons.hpp"

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

	SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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
	textBox->onValueChangedT = [](std::string a, std::string b) {
		printf("%s \n%s \n ====\n", a.c_str(), b.c_str());
	};
	gui.add(textBox);

	auto check = std::make_shared<edui::Checkbox>();
	check->setSize({0, 200}, {0, 26});
	check->setPosition({0, 20}, {0, 100});
	check->onValueChangedT = [](bool previous, bool current) {
		printf("%i, %i \n", previous, current);
	};
	gui.add(check);

	auto intval = std::make_shared<edui::IntValue>();
	intval->setSize({0, 200}, {0, 26});
	intval->setPosition({0, 20}, {0, 150});
	intval->onValueChangedT = [](int previous, int current) {
		printf("%i, %i \n", previous, current);
	};
	gui.add(intval);

	/*
	auto dropmenu = std::make_shared<edui::DropdownList>();
	dropmenu->setPosition({0, 20}, {0, 200});
	dropmenu->setSize({0, 200}, {0, 200});
	dropmenu->addItem("Hello");
	gui.add(dropmenu);
	*/

	auto dropdown = std::make_shared<edui::Dropdown>();
	dropdown->setPosition({0, 20}, {0, 200});
	dropdown->setSize({0, 200}, {0, 32});
	dropdown->addItem("Hello");
	dropdown->addItem("Hello2");
	gui.add(dropdown);

	while (!WindowShouldClose()) {
		gui.update();

		BeginDrawing();

		gui.draw();

		//DrawRectangle(0, 200, 16, 16, GREEN);
		//GuiDrawIcon(ICON_ARROW_DOWN_FILL, 0, 200, 1, BLACK);

		EndDrawing();
	}

	gui.unload();

	CloseWindow();

	printf("Closing editor..\n");

	return 0;
}
