#include <cstdio>
#include <memory>
#include <string>

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/canvas.hpp"
#include "edui/checkbox.hpp"
#include "edui/childWindow.hpp"
#include "edui/colorRect.hpp"
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
#include "edui/rliconsarr.hpp"
#include "edui/tabBar.hpp"
#include "edui/textArea.hpp"
#include "edui/textBox.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/dropdown.hpp"
#include "edui/widget.hpp"
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

	auto intval = std::make_shared<edui::IntValue>();
	intval->setSize({0, 200}, {0, 26});
	intval->setPosition({0, 20}, {0, 150});
	intval->onValueChangedT = [](int previous, int current) {
		printf("%i, %i \n", previous, current);
	};
	gui.add(intval);

	auto tabContent = std::make_shared<edui::Container>();
	tabContent->setPosition({0, 300}, {0, 44});
	tabContent->setSize({0, 300}, {0, 300});
	//gui.add(tabContent);

	auto tabBar = std::make_shared<edui::FileTabBar>();
	tabBar->contentBase = tabContent;
	tabBar->setPosition({0, 300}, {0, 22});
	tabBar->setSize({0, 300}, {0, 22});
	//gui.add(tabBar);
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
	msg->onOkPressed = [] {
		printf("clicked ok..\n");
	};
	gui.add(msg);

	auto hori = std::make_shared<edui::HorizontalContainer>();
	hori->render->as<edui::HorizontalContainerRender>().space = 2;
	hori->setSize({0, 200}, {0, 30});
	auto item1 = std::make_shared<edui::Label>();
	item1->setSize({0, 30}, {1, 0});
	hori->add(item1);
	auto item2 = std::make_shared<edui::Label>();
	item2->setSize({0, 30}, {1, 0});
	hori->add(item2);
	gui.add(hori);

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
