#include <cstddef>
#include <cstdio>
#include <memory>
#include <string>

#define STR_IMPLEMENTATION

#include "editor.hpp"
#include "edui/button.hpp"
#include "edui/canvas.hpp"
#include "edui/checkbox.hpp"
#include "edui/childWindow.hpp"
#include "edui/colorPanel.hpp"
#include "edui/colorRect.hpp"
#include "edui/colorValue.hpp"
#include "edui/colorWheel.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/container.hpp"
#include "edui/contextMenu.hpp"
#include "edui/dropdown.hpp"
#include "edui/dropdownList.hpp"
#include "edui/fileTabBar.hpp"
#include "edui/gui.hpp"
#include "edui/helper.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/horizontalScrollbar.hpp"
#include "edui/iconTextButton.hpp"
#include "edui/intValue.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/languageDropdown.hpp"
#include "edui/menuBar.hpp"
#include "edui/messageBox.hpp"
#include "edui/rlicons.hpp"
#include "edui/rliconsarr.hpp"
#include "edui/slider.hpp"
#include "edui/tabBar.hpp"
#include "edui/textArea.hpp"
#include "edui/textBox.hpp"
#include "edui/textEdit.hpp"
#include "edui/vector2Value.hpp"
#include "edui/verticalContainer.hpp"
#include "edui/verticalScrollbar.hpp"
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

	Color out = parseColorString("245,50,50");
	printf("%u, %u, %u\n", out.r, out.g, out.b);

	edui::Gui gui;

	gui.loadThemesDir("resources/themes");
	gui.loadTheme("RPG++ Light");

	gui.loadTranslationNames("resources/langs.ini");
	gui.loadTranslation("resources/en.ini");

	gui.setFont("resources/TerminusTTF-4.49.3.ttf", 96, 18, 0);

	auto menuBar = std::make_shared<edui::MenuBar>();
	menuBar->translationId = "Menu";
	menuBar->addItem("file", {"one", "two"});
	menuBar->onItemClicked.connect(
		[](const std::string &title, const std::string &option) { printf("%s %s \n", title.c_str(), option.c_str()); });
	menuBar->onItemClickedInt.connect([](size_t a, size_t b) { printf("%zu %zu \n", a, b); });
	gui.addMenuBar(menuBar);

	auto label = std::make_shared<edui::Label>();
	label->translationId = "Widgets.hello";
	label->setSize({0, 100}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	gui.add(label);

	auto slider = std::make_shared<edui::Slider>();
	slider->setPosition({0, 20}, {0, 120});
	slider->setSize({0, 200}, {0, 26});
	slider->onValueChanged.connect([](int newVal) { printf("slider change: %i \n", newVal); });
	gui.add(slider);

	auto textEdit = std::make_shared<edui::TextEdit>();
	textEdit->setPosition({0, 20}, {0, 150});
	textEdit->setSize({0, 300}, {0, 250});
	textEdit->setText("Hello world!\nLazy fox.\nThird way.");
	gui.add(textEdit);

	auto colorValue = std::make_shared<edui::ColorValue>();
	colorValue->setPosition({0, 350}, {0, 300});
	colorValue->setSize({0, 50}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	colorValue->onValueChangedT([](Color old, Color newColor) {
		printf("%u, %u, %u ; %u, %u, %u\n", old.r, old.g, old.b, newColor.r, newColor.g, newColor.b);
	});
	gui.add(colorValue);

	auto langWindow = std::make_shared<edui::ChildWindow>();
	langWindow->translationId = "Editor.window";
	langWindow->setTitle("Choose Language..");
	langWindow->setPosition({0, 350}, {0, 20});
	langWindow->setSize({0, 200}, {0, 180});
	gui.add(langWindow);

	auto langBox = std::make_shared<edui::LanguageDropdown>();
	langBox->setSize({1, 0}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	langBox->onValueChangedT([](edui::DropdownValue old, edui::DropdownValue newValue) {
		auto langName = std::string(edui::Gui::instance->languageNames[newValue.idx].key.c_str());
		std::string sourceFile = TextFormat("resources/%s.ini", langName.c_str());
		edui::Gui::instance->loadTranslation(sourceFile);
	});
	langWindow->add(langBox);

	auto testDrop = std::make_shared<edui::Dropdown>();
	testDrop->translationId = "Editor.drop";
	testDrop->setPosition({0, 0}, {0.5f, 0});
	testDrop->setSize({1, 0}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	testDrop->addItem("one");
	testDrop->addItem("two");
	langWindow->add(testDrop);

	auto themeDrop = std::make_shared<edui::Dropdown>();
	themeDrop->setPosition({0, 0}, {0, 30});
	themeDrop->setSize({1, 0}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	for (int i = 0; i < edui::Gui::instance->themesCount; i++) {
		auto entry = edui::Gui::instance->themeEntries[i];
		themeDrop->addItem(entry.value.c_str());
		if (TextIsEqual(entry.key.c_str(), "theme.ini")) {
			themeDrop->setValue({i, ""});
		}
	}
	themeDrop->onValueChangedT(
		[](edui::DropdownValue old, edui::DropdownValue newValue) { edui::Gui::instance->loadTheme(newValue.val); });
	langWindow->add(themeDrop);

	auto scrollbar = std::make_shared<edui::VerticalScrollbar>();
	scrollbar->setPosition({0, 560}, {0, 20});
	scrollbar->setSize({0, static_cast<int>(edui::EDUI_DEFAULT_HEIGHT)}, {0, 180});
	scrollbar->setValue(20);
	// gui.add(scrollbar);

	auto horizontalContainer = std::make_shared<edui::HorizontalContainer>();
	horizontalContainer->setPosition({0, 560}, {0, 20});
	horizontalContainer->setSize({0, 200}, {0, 180});
	for (int i = 0; i < 20; i++) {
		auto newButton = std::make_shared<edui::Button>();
		newButton->setSize({0, 40}, {1, 0});
		newButton->setText(TextFormat("Button%i", i));
		horizontalContainer->add(newButton);
	}
	gui.add(horizontalContainer);

	auto horizontalScrollbar = std::make_shared<edui::HorizontalScrollbar>();
	horizontalScrollbar->setPosition({0, 560}, {0, 20});
	horizontalScrollbar->setSize({0, 200}, {0, static_cast<int>(edui::EDUI_DEFAULT_HEIGHT)});
	// gui.add(horizontalScrollbar);

	auto fileContent = std::make_shared<edui::Container>();

	auto fileList = std::make_shared<edui::FileTabBar>();
	fileList->contentBase = fileContent;
	fileList->setPosition({0, 560}, {0, 204});
	fileList->setSize({0, 200}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	fileList->addItem("item1", 0);
	fileList->addItem("item2", 0);
	fileList->addItem("item3", 0);
	gui.add(fileList);

	auto tabContent = std::make_shared<edui::Container>();
	tabContent->setPosition({0, 560}, {0, 264});
	tabContent->setSize({0, 200}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	gui.add(tabContent);

	auto tabList = std::make_shared<edui::TabBar>();
	tabList->translationId = "Editor.tabs";
	tabList->contentBase = tabContent;
	tabList->setPosition({0, 560}, {0, 234});
	tabList->setSize({0, 200}, {0, static_cast<int>(edui::EDUI_SECONDARY_HEIGHT)});
	tabList->addItem("item1", 0);
	tabList->addItem("item2", 0);
	gui.add(tabList);

	tabList->showLastTab();

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
