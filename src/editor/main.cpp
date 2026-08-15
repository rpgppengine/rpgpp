#include <cstdio>
#include <memory>
#include <string>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/canvas.hpp"
#include "edui/checkbox.hpp"
#include "edui/childWindow.hpp"
#include "edui/colorRect.hpp"
#include "edui/container.hpp"
#include "edui/contextMenu.hpp"
#include "edui/dropdown.hpp"
#include "edui/dropdownList.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/intValue.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/menuBar.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/dropdown.hpp"
#include "gamedata.hpp"
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

	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "raylib example - basic window");
	SetTargetFPS(60);

	edui::Gui gui;

	gui.setFont("resources/TerminusTTF-4.49.3.ttf", 96, 18, 0);

	auto menuBar = std::make_shared<edui::MenuBar>();
	gui.addMenuBar(menuBar);
	menuBar->addItem("Menu1", {"Item1", "Item2"});
	menuBar->onItemClicked = [] (const std::string& title, const std::string& option) {
		printf("%s %s \n", title.c_str(), option.c_str());
	};

	auto label = std::make_shared<edui::Label>();
	label->setSize({1, -20}, {0, 26});
	label->setText("Hello world!");
	label->render->as<edui::LabelRender>().padding = 2;
	//label->render->as<edui::LabelRender>().vertAlign = edui::VerticalAlignment::TEXT_BOTTOM;
	//label->render->as<edui::LabelRender>().fontSize = 0.125f;
	gui.add(label);

	auto textBox = std::make_shared<edui::TextBox>();
	textBox->setSize({0, 200}, {0, 26});
	textBox->setPosition({0, 20}, {0, 50});
	textBox->setText("Hello World! The lazy fox jumped over the dog!");
	//textBox->render->as<edui::TextBoxRender>().fontSize = 2.0f;
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

	auto dropdown = std::make_shared<edui::Dropdown>();
	dropdown->setPosition({0, 20}, {0, 200});
	dropdown->setSize({0, 200}, {0, 26});
	dropdown->addItem("Hello");
	dropdown->addItem("Hello2");
	gui.add(dropdown);

	auto childWindow = std::make_shared<edui::ChildWindow>();
	childWindow->setPosition({0, 240}, {0, 20});
	childWindow->setSize({0, 200}, {0, 200});
	childWindow->setTitle("ChildWindow");
	gui.add(childWindow);

	auto label2 = std::make_shared<edui::IconTextButton>();
	label2->anchor = {0.5f, 0.5f};
	label2->setSize({0, 160}, {0, 26});
	label2->iconId = ICON_FILETYPE_TEXT;
	label2->setText("Hello");
	label2->render->as<edui::LabelRender>().padding = 2;
	//label2->render->as<edui::LabelRender>().fontSize = 2.0f;
	//childWindow->add(label2);
	//
	auto canvas = std::make_shared<edui::Canvas>();
	canvas->setSize({1, 0}, {1, 0});
	childWindow->add(canvas);

	auto context = std::make_shared<edui::ContextMenu>();
	context->setPosition({0, 500}, {0, 20});
	context->setSize({0, 200}, {0, 0});
	context->addItem("Item1");
	context->addItem("Item2");
	context->onItemClicked = [](const std::string& item) {
		printf("%s \n", item.c_str());
	};
	gui.add(context);

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
