#include "services/editorGuiService.hpp"

#include <TGUI/AllWidgets.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>

#include "TGUI/Backend/raylib.hpp"
#include "TGUI/Loading/Theme.hpp"
#include "TGUI/ObjectConverter.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "screens/guiScreen.hpp"
#include "screens/welcomeScreen.hpp"
#include "services/childWindowSubService.hpp"
#include "services/translationService.hpp"
#include "updatable.hpp"

constexpr int BASE_WINDOW_WIDTH = 800;
constexpr int BASE_WINDOW_HEIGHT = 600;
constexpr float GRADIENT_SPEED_MUTLIPLIER = 0.3f;
constexpr float GRADIENT_COLOR_MULTIPLIER = 40.0f;

EditorGuiService::EditorGuiService() {
	isResettingUI = false;
	gui = std::unique_ptr<tgui::Gui>();
	currentScreen = std::unique_ptr<UIScreen>();
	childWindowService = std::unique_ptr<ChildWindowSubService>();
}

void EditorGuiService::init() {
	currentCursor = MOUSE_CURSOR_DEFAULT;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(BASE_WINDOW_WIDTH, BASE_WINDOW_HEIGHT, "RPG++ Editor");
	InitAudioDevice();

	font = LoadFontEx("resources/LanaPixel.ttf", 13, 0, 256);

	auto &cfgs = Editor::instance->getConfiguration();
	auto &hks = Editor::instance->getHotkeyService();

	hks.deserialize(cfgs.getField("hotkeys"));

	this->resetUi();
	hks.registerHotkeyCallback("toggle_debug", [this]() { perfOverlay.Toggle(); });
	hks.registerHotkeyCallback("new_project", [] { Editor::instance->getFs().promptNewProject(); });
	hks.registerHotkeyCallback("open_project", [] { Editor::instance->getFs().promptOpenProject(); });
}

void EditorGuiService::resetUi() {
	ThemeService &theme = Editor::instance->getThemeService();
	ConfigurationService &configService = Editor::instance->getConfiguration();

	this->isResettingUI = false;
	// Check if we already have a gui already setup, if we do.. don't load
	// the same assets again.
	if (this->gui == nullptr) {
		this->gui = std::make_unique<tgui::Gui>();
		Editor::setAppIcon(RPGPP_EXECUTABLE_LOGO);
	}

	string configTheme;

	try {
		configTheme = configService.getStringValue("theme");
	} catch (const std::exception &e) {
		configTheme = "Dark";
	}

	theme.setTheme(configTheme);
	// NOTE: The theme is not set properly if this is not initialized after it.
	if (this->childWindowService != nullptr)
		this->childWindowService->createWindows();
	else
		this->childWindowService = std::make_unique<ChildWindowSubService>();

	initMenuBar();

	auto group = tgui::Group::create({"100%"});
	group->setPosition(0, MENUBAR_H);
	group->setSize({"100%", tgui::Layout("100%") - MENUBAR_H});

	gui->add(group);

	screenContainer = group;

	this->setScreen(std::make_unique<screens::WelcomeScreen>(), true);
}

void EditorGuiService::uiLoop() {
	auto const &cg = this->gui;
	SetTraceLogLevel(LOG_WARNING);

	tgui::Theme::addRendererInheritanceParent("NewProjectWindow", "ChildWindow");
	tgui::Theme::addRendererInheritanceParent("RoomToolbox", "Tabs");
	// main loop.
	while (!WindowShouldClose()) {
		perfOverlay.Update();

		cg->handleEvents();
		int pressedChar = GetCharPressed();
		while (pressedChar) {
			cg->handleCharPressed(pressedChar);
			pressedChar = GetCharPressed();
		}
		int pressedKey = GetKeyPressed();
		while (pressedKey) {
			Editor::instance->getHotkeyService().fire();
			cg->handleKeyPressed(pressedKey);
			pressedKey = GetKeyPressed();
		}

		for (const auto &widget : updatableWidgets) {
			if (!widget.expired()) {
				const std::shared_ptr<IUpdatable> spt = widget.lock();
				spt->update();
			}
		}

		if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
			leftMouseHeld = true;
		} else {
			if (leftMouseHeld) {
				currentScreen->leftMouseReleased(GetMouseX(), GetMouseY() - MENUBAR_H);
				leftMouseHeld = false;
			}
		}
		currentScreen->mouseMove(GetMouseX(), GetMouseY() - MENUBAR_H);
		BeginDrawing();
		ClearBackground(DARKGRAY);

		auto bgProp = tgui::Theme::getDefault()->getGlobalProperty("BackgroundColorDisabled");
		if (bgProp.getType() == tgui::ObjectConverter::Type::None) {
			bgProp = tgui::Theme::getDefault()->getGlobalProperty("BackgroundColor");
		}
		auto bgColor = bgProp.getColor();
		auto topGradientColor =
			static_cast<unsigned char>(abs(sin(GetTime() * GRADIENT_SPEED_MUTLIPLIER)) * GRADIENT_COLOR_MULTIPLIER);
		DrawRectangle(0, 0, GetRenderWidth(), GetRenderHeight(),
					  {bgColor.getRed(), bgColor.getGreen(), bgColor.getBlue(), 255});
		cg->draw();
		// Due to many reasons, one time... Thefirey33 decided to talk
		// to the C++ MSVC Compiler if he can reset the current state of
		// his world at any time. MSVC replied, "MEMORY EXCEPTION".
		// Thefirey33 cried, pleaded... his niko-like eyes looking at
		// MSVC. But the solution was... to put a boolean to tell the
		// loop to reset it with an if statement. NOTE: If you think of a
		// better solution. Too Bad!
		if (isResettingUI) {
			this->resetUi();
			this->childWindowService->createWindows();
		}

		perfOverlay.Draw(20, 40, 400, 100);
		EndDrawing();
	}
	this->childWindowService.reset();
	currentScreen.reset();
	gui.reset();
}

void EditorGuiService::setScreen(std::unique_ptr<UIScreen> setToScreen, bool forceSwitch) {
	if (this->currentScreen != nullptr && setToScreen->getNameOfScreen() == this->currentScreen->getNameOfScreen() &&
		!forceSwitch) {
		return;
	}
	auto group = this->uiChangePreInit(setToScreen.get());
	this->screenHistory.push_back(std::move(this->currentScreen));
	this->currentScreen.swap(setToScreen);
	this->currentScreen->initItems(group);
}

/**
 * this function basically handles the pre-phase before the screen changes and
 * gets everything ready for the new screen.
 * @param setToScreen the screen we are currently changing to.
 * @return the group that the screen must use to position and add it's widgets.
 */
tgui::Group::Ptr EditorGuiService::uiChangePreInit(UIScreen *setToScreen) {
	if (screenContainer.expired()) {
		throw std::runtime_error("Screen container's weak pointer is expired");
	}

	screenContainer.lock()->removeAllWidgets();

	if (this->currentScreen != nullptr) this->currentScreen->unloadScreen();
	std::string title = "RPG++ Editor - ";
	title.append(setToScreen->getNameOfScreen());
	SetWindowTitle(title.c_str());
	return screenContainer.lock();
}

void EditorGuiService::addUpdate(std::shared_ptr<IUpdatable> widget) { updatableWidgets.push_back(widget); }

void EditorGuiService::setScreen(UIScreen *setToScreen, bool forceSwitch) {
	if (this->currentScreen != nullptr && setToScreen->getNameOfScreen() == this->currentScreen->getNameOfScreen() &&
		!forceSwitch) {
		return;
	}
	auto group = this->uiChangePreInit(setToScreen);
	this->screenHistory.push_back(std::move(this->currentScreen));
	this->currentScreen = std::unique_ptr<UIScreen>(setToScreen);
	this->currentScreen->initItems(group);
	gui->add(group);
}

void EditorGuiService::createLogoCenter(const tgui::GrowVerticalLayout::Ptr &layout) {
	const auto boxLayout = tgui::BoxLayout::create({"100%", 96});
	std::string logoPath = Editor::instance->getFs().getResourcePath("logo-ups.png");
	const auto welcomePic = tgui::Picture::create(logoPath.c_str());
	welcomePic->setOrigin({0.5, 0.5});
	welcomePic->setPosition({"50%", "50%"});
	boxLayout->add(welcomePic);

	layout->add(boxLayout);
}

void EditorGuiService::reloadUi() {
	/*
	auto group = this->uiChangePreInit(this->currentScreen.get());
	this->currentScreen->initItems(group);
	gui->add(group);
	this->childWindowService->createWindows();
	*/
}

ChildWindowSubService *EditorGuiService::getChildWindowSubService() { return this->childWindowService.get(); }

void EditorGuiService::initMenuBar() {
	auto menuBarPtr = tgui::MenuBar::create();
	this->menuBar = menuBarPtr;
	auto &ts = Editor::instance->getTranslations();

	const auto &fileT = ts.getKey("menu.file._label");
	const auto &fileOpenProjectT = ts.getKey("menu.file.open_project");
	const auto &fileNewProjectT = ts.getKey("menu.file.new_project");

	menuBarPtr->addMenu(fileT);
	menuBarPtr->addMenuItem(fileNewProjectT);
	menuBarPtr->connectMenuItem({fileT, fileNewProjectT}, [] { Editor::instance->getFs().promptNewProject(); });
	menuBarPtr->addMenuItem(fileOpenProjectT);
	menuBarPtr->addMenuItem(ts.getKey("menu.file.save_file"));
	menuBarPtr->connectMenuItem({fileT, fileOpenProjectT}, [] { Editor::instance->getFs().promptOpenProject(); });

	menuBarPtr->addMenu(ts.getKey("menu.edit._label"));
	menuBarPtr->addMenuItem(ts.getKey("menu.edit.undo"));
	menuBarPtr->addMenuItem(ts.getKey("menu.edit.redo"));

	const auto optionsTranslation = ts.getKey("menu.options._label");
	const auto editorOptionsTranslation = ts.getKey("menu.options.editor");
	menuBarPtr->addMenu(optionsTranslation);
	menuBarPtr->addMenuItem(editorOptionsTranslation);
	menuBarPtr->connectMenuItem({optionsTranslation, editorOptionsTranslation},
								[&] { this->childWindowService->openWindow("options"); });

	const auto &aboutOptions = ts.getKey("menu.about._label"), &aboutRpgpp = ts.getKey("menu.about.rpgpp");
	menuBarPtr->addMenu(aboutOptions);
	menuBarPtr->addMenuItem(aboutRpgpp);

	menuBarPtr->connectMenuItem({aboutOptions, aboutRpgpp}, [&] { this->childWindowService->openWindow("about"); });
	menuBarPtr->setSize({"100%", MENUBAR_H});
	this->gui->add(menuBarPtr);
}

void EditorGuiService::gotoPreviousScreen() {
	if (!this->screenHistory.empty()) {
		std::unique_ptr<UIScreen> lastScreen = std::move(this->screenHistory.back());
		this->screenHistory.pop_back();

		auto group = this->uiChangePreInit(lastScreen.get());
		this->currentScreen = std::move(lastScreen);
		this->currentScreen->initItems(group);
		gui->add(group);
	}
}

void EditorGuiService::centerWidget(tgui::Widget::Ptr widget) {
	widget->setPosition((GetScreenWidth() / 2.0f) - (widget->getSize().x / 2.0f),
						(GetScreenHeight() / 2.0f) - (widget->getSize().y / 2.0f));
}

void EditorGuiService::alert(tgui::String title, tgui::String content) {
	tgui::MessageBox::Ptr dialog = tgui::MessageBox::create();
	dialog->setText(content);
	dialog->setTitle(title);
	dialog->addButton("Ok");
	dialog->onButtonPress([this, dialog]() { gui->remove(dialog); });
	gui->add(dialog);
	centerWidget(dialog);
}

Font EditorGuiService::getFont() { return font; }

void EditorGuiService::unload() { UnloadFont(font); }
