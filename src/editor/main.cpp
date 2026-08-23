#include <cstdio>
#include <memory>
#include <string>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/canvas.hpp"
#include "edui/checkbox.hpp"
#include "edui/childWindow.hpp"
#include "edui/colorRect.hpp"
#include "edui/colorWheel.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/container.hpp"
#include "edui/contextMenu.hpp"
#include "edui/dropdown.hpp"
#include "edui/dropdownList.hpp"
#include "edui/fileTabBar.hpp"
#include "edui/gui.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/intValue.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/menuBar.hpp"
#include "edui/messageBox.hpp"
#include "edui/rlicons.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/slider.hpp"
#include "edui/tabBar.hpp"
#include "edui/textArea.hpp"
#include "edui/textBox.hpp"
#include "edui/vector2Value.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/widget.hpp"
#include "gamedata.hpp"
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

	SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_RESIZABLE);
	InitWindow(800, 450, "raylib example - basic window");
	SetTargetFPS(60);

	edui::Gui gui;

	gui.setFont("resources/TerminusTTF-4.49.3.ttf", 96, 18, 0);

	auto menuBar = std::make_shared<edui::MenuBar>();
	gui.addMenuBar(menuBar);
	menuBar->addItem("Menu1", {"Item1", "Item2"});
	menuBar->onItemClicked.connect(
		[](const std::string &title, const std::string &option) { printf("%s %s \n", title.c_str(), option.c_str()); });

	auto intval = std::make_shared<edui::IntValue>();
	intval->setSize({0, 200}, {0, 26});
	intval->setPosition({0, 20}, {0, 150});
	intval->onValueChangedT.connect([](int previous, int current) { printf("%i, %i \n", previous, current); });
	gui.add(intval);

	auto tabContent = std::make_shared<edui::Container>();
	tabContent->setPosition({0, 300}, {0, 44});
	tabContent->setSize({0, 300}, {0, 300});
	// gui.add(tabContent);

	auto tabBar = std::make_shared<edui::FileTabBar>();
	tabBar->contentBase = tabContent;
	tabBar->setPosition({0, 300}, {0, 22});
	tabBar->setSize({0, 300}, {0, 22});
	// gui.add(tabBar);
	auto screenRect = gui.getScreenRect();
	tabBar->calcRect(screenRect);

	auto page1 = tabBar->addItem("Hello", ICON_FILE);

	auto pageLabel = std::make_shared<edui::Label>();
	pageLabel->setSize({0.5f, 0}, {0, 22});
	page1->add(pageLabel);

	auto page2 = tabBar->addItem("Hello2", ICON_FILETYPE_BINARY);
	auto pageLabel2 = std::make_shared<edui::Label>();
	pageLabel2->setText("Page 2");
	pageLabel2->setSize({0.5f, 0}, {0, 22});
	page2->add(pageLabel2);

	auto page3 = tabBar->addItem("Hello3", 0);
	tabBar->addItem("Test Settings", ICON_INFO);
	auto pageLabel3 = std::make_shared<edui::Label>();
	pageLabel3->setText("Page 3");
	pageLabel3->setSize({0.5f, 0}, {0, 22});
	page3->add(pageLabel3);

	auto msg = std::make_shared<edui::ConfirmDialog>();
	msg->setTitle("ConfirmDialog");
	msg->setText("I have a message.");
	msg->setPosition({0, 250}, {0, 20});
	msg->setSize({0, 200}, {0, 180});
	msg->onOkPressed.connect([] { printf("clicked ok..\n"); });
	gui.add(msg);

	auto vec2 = std::make_shared<edui::Vector2Value>();
	vec2->setSize({0, 200}, {0, 26});
	vec2->setPosition({0, 20}, {0, 190});
	vec2->onValueChangedT.connect([](Vector2 old, Vector2 newVec) {
		printf("old: %f, %f \n", old.x, old.y);
		printf("new: %f, %f \n", newVec.x, newVec.y);
	});
	gui.add(vec2);

	auto colorWheel = std::make_shared<edui::ColorWheel>();
	colorWheel->setSize({0, 100}, {0, 100});
	colorWheel->setColor(PINK);
	colorWheel->onColorChanged.connect([](Color c) { printf("%i %i %i \n", c.r, c.g, c.b); });
	gui.add(colorWheel);

	auto slider = std::make_shared<edui::Slider>();
	slider->setPosition({0, 20}, {0, 120});
	slider->setSize({0, 200}, {0, 26});
	slider->onValueChanged.connect([](int newVal) { printf("slider change: %i \n", newVal); });
	gui.add(slider);

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
