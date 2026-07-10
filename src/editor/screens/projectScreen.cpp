#include "screens/projectScreen.hpp"

#include <cassert>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <vector>

#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/BitmapButton.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/MenuBar.hpp"
#include "TGUI/Widgets/MessageBox.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "TGUI/Widgets/Scrollbar.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "bindTranslation.hpp"
#include "components/resizableContainer.hpp"
#include "components/tooltip.hpp"
#include "editor.hpp"
#include "projectFile.hpp"
#include "raylib.h"
#include "services/editorGuiService.hpp"
#include "services/fileSystemService.hpp"
#include "services/translationService.hpp"
#include "widgets/newFileDialog.hpp"
using namespace screens;
void ProjectScreen::layoutReload() { resListWBinder->setSize(modifiable_RESLIST_W, "100%"); }

void ProjectScreen::mouseMove(int x, int y) {
	resourcesList->manualMouseMoved({static_cast<float>(x), static_cast<float>(y)});
	fileTabs->manualMouseMoved(
		{static_cast<float>(
			 x										// coordinate of the mouse cursor relative to the projectScreen
			 - tabsContainer->getPosition().x		// coordinate of the tabsContainer
													// relative to the projectScreen
			 + tabsContainer->getContentOffset().x	// coordinate of the widget relative to the tabsContainer
			 // why all of these calculations? cause the builtin
			 // leftMousePressed method returns the mouse coordinate relative to
			 // the current widget
			 ),
		 static_cast<float>(y - tabsContainer->getPosition().y)});
}
void ProjectScreen::leftMouseReleased(int x, int y) {
	resourcesList->manualLeftMouseReleased({static_cast<float>(x), static_cast<float>(y)});
	fileTabs->manualLeftMouseReleased(
		{// ditto
		 static_cast<float>(x - tabsContainer->getPosition().x + tabsContainer->getContentOffset().x),
		 static_cast<float>(y - tabsContainer->getPosition().y)});
}

void ProjectScreen::bindMenuBarAndHK(tgui::MenuBar::Ptr menuBarPtr) {
	auto &ts = Editor::instance->getTranslations();
	auto &hks = Editor::instance->getHotkeyService();

	auto saveAction = [this] {
		if (!openedFiles.empty()) {
			tgui::String currentFile = fileTabs->getSelectedId();
			auto &projectFile = openedFiles.at(currentFile);
			projectFile->saveFile(projectFile->getFilePath());
			projectFile->getView().dirty = false;
		}
	};

	auto undoAction = [this] { getCurrentFile().getView().undoAction(); };

	auto redoAction = [this] { getCurrentFile().getView().redoAction(); };

	std::vector<tgui::String> saveFileHierarchy = {ts.getKey("menu.file._label"), ts.getKey("menu.file.save_file")};
	std::vector<tgui::String> undoHierarchy = {ts.getKey("menu.edit._label"), ts.getKey("menu.edit.undo")};
	std::vector<tgui::String> redoHierarchy = {ts.getKey("menu.edit._label"), ts.getKey("menu.edit.redo")};
	menuBarPtr->setMenuItemEnabled(saveFileHierarchy, true);
	menuBarPtr->connectMenuItem(saveFileHierarchy, saveAction);

	menuBarPtr->setMenuItemEnabled(undoHierarchy, true);
	menuBarPtr->connectMenuItem(undoHierarchy, undoAction);

	menuBarPtr->setMenuItemEnabled(redoHierarchy, true);
	menuBarPtr->connectMenuItem(redoHierarchy, redoAction);

	hks.registerHotkeyCallback("save_file", saveAction);
	hks.registerHotkeyCallback("undo", undoAction);
	hks.registerHotkeyCallback("redo", redoAction);
}

void ProjectScreen::initItems(tgui::Group::Ptr layout) {
	if (auto ptr = Editor::instance->getGui().menuBar.lock()) {
		bindMenuBarAndHK(ptr);
	}

	// Commentary:
	// So while I was re-designing the project screen, I came across this single
	// Tabs widget that's not being used anywhere. Not even once. And I thought
	// to myself: "Well this seems useless". So I deleted it, and for some
	// reason, the entire theming for Tabs in the screen got fcked. At first, I
	// thought it could be something related to me changing the order of widgets
	// being declared. Then I thought that the ThemeService had some bugs, so I
	// investigated but found nothing. After that, I decided to revert my
	// progress on this file, at which point, I figured out pretty quickly that
	// this one variable is the sole reason why the theming got fcked.
	//
	// So... yea, for future developers, if you're reading this: Please don't
	// delete this seemingly extra widget. It's what keeping the project screen
	// together.
	auto _tab = tgui::Tabs::create();

	auto &ts = Editor::instance->getTranslations();

	fileContextMenu = tgui::ContextMenu::create();
	bindTranslationWithCallback<tgui::ContextMenu>(fileContextMenu,
												   [](std::shared_ptr<tgui::ContextMenu> menu, TranslationService &ts) {
													   menu->removeAllMenuItems();
													   menu->addMenuItem(ts.getKey("context_menu.copy_full_path"));
													   menu->addMenuItem(ts.getKey("context_menu.delete"));
												   });
	Editor::instance->getGui().gui->add(fileContextMenu);

	openedFiles = std::map<tgui::String, std::unique_ptr<ProjectFile>>{};
	fileVisitor = std::make_unique<ProjectFileVisitor>();
	fileInitVisitor = std::make_unique<FileInitVisitor>();
	listedResourcesType = EngineFileType::FILE_MAP;

	// For allowing widgets to hook into the width of the resizable resource
	// list Must ALWAYS be initialized first
	resListWBinder = tgui::Group::create({modifiable_RESLIST_W, 0});
	layout->add(resListWBinder, "resListWBinder");

	// Resource list
	resourcesList = createResourcesList();
	layout->add(resourcesList);

	// Tool bar
	toolBar = createToolBar();
	layout->add(toolBar);

	// File tabs
	tabsContainer = tgui::ScrollablePanel::create();
	tabsContainer->getRenderer()->setBorders({0, 0, 0, 0});
	tabsContainer->getRenderer()->setRoundedBorderRadius(0);
	tabsContainer->getRenderer()->setPadding(0);
	tabsContainer->setSize(tgui::Layout("100%") - tgui::bindWidth(resListWBinder), FILETABS_H);
	tabsContainer->setPosition(tgui::bindWidth(resListWBinder), tgui::bindBottom(toolBar));
	tabsContainer->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);
	tabsContainer->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

	fileTabs = FileTab::create();
	fileTabs->setHeight(FILETABS_H);
	fileTabs->setPosition({0, 0});
	fileTabs->useExternalMouseEvent = true;
	fileTabs->setAutoSize(true);
	fileTabs->setMinimumTabWidth(MIN_TAB_W_IN_FILETABS);
	fileTabs->setMaximumTabWidth(MAX_TAB_W_IN_FILETABS);

	fileTabs->onTabClose([this](tgui::String id) {
		openedFiles.erase(id);
		if (fileTabs->getTabsCount() == 0) {
			clearView();
		}
	});
	fileTabs->onTabSelect([this](tgui::String id) {
		if (fileTabs->getTabsCount() == 0) {
			clearView();
		} else {
			switchView(id);
		}
	});

	Editor::instance->getHotkeyService().registerHotkeyCallback("close_tab", [this] { fileTabs->closeCurrentTab(); });

	tabsContainer->add(fileTabs);
	layout->add(tabsContainer);

	// File view
	auto fileView =
		tgui::Group::create({tgui::Layout("100%") - tgui::bindWidth(resListWBinder),
							 tgui::Layout("100%") - tgui::bindHeight(toolBar) - tgui::bindHeight(tabsContainer)});
	fileView->setPosition(tgui::bindWidth(resListWBinder), tgui::bindBottom(tabsContainer));
	this->fileViewGroup = fileView;
	clearView();
	layout->add(fileView);

	// Maximize when a project is opened
	SetWindowState(FLAG_WINDOW_MAXIMIZED);

	// FIXME: fix scaling issue on windows!
}

void ProjectScreen::addFileView(EngineFileType fileType, const std::string &path) {
	std::string fileName = GetFileName(path.c_str());

	if (!FileExists(path.c_str())) {
		// NOTE: This is a fix for when the user accidentally deletes a file in the project root directory,
		// so the engine doesn't try opening a now deleted file.
		auto &ts = Editor::instance->getTranslations();

		auto fileNotExistMsgbox = tgui::MessageBox::create();
		fileNotExistMsgbox->setPosition({"50%", "50%"});
		fileNotExistMsgbox->setOrigin({0.5, 0.5});

		bindTranslation(fileNotExistMsgbox, "screen.project.filenotexist.notice", &tgui::MessageBox::setText);
		fileNotExistMsgbox->addButton(ts.getKey("button.okay"));

		std::weak_ptr<tgui::MessageBox> weakMsgbox = fileNotExistMsgbox;
		fileNotExistMsgbox->onButtonPress.connect([this, fileName, weakMsgbox]() {
			if (auto box = weakMsgbox.lock()) {
				box->close();
				this->addResourceButtons(this->listedResourcesType);
				fileTabs->closeTabFilename(fileName);
			}
		});

		Editor::instance->getGui().gui->add(fileNotExistMsgbox);
		return;
	}

	Editor::instance->getGui().gui->setTabKeyUsageEnabled(fileType != EngineFileType::FILE_SCRIPT);

	std::unique_ptr<ProjectFile> projectFile = fileVisitor->visit(fileType, path);
	if (projectFile->isEmpty) {
		std::string mutPath = std::string(path);
		Editor::instance->getFs().openFileInDefaultApp(mutPath);
		return;
	}
	auto idx = fileTabs->addFileTab(path, fileName, projectFile.get()->getViewPtr());

	if (idx != -1) {
		fileViewGroup->removeAllWidgets();

		projectFile->initUi(fileViewGroup);
		projectFile->setFilePath(path);
		tgui::String id = path;
		focusedFile = id;
		projectFile->getView().fileViewFocused = true;
		openedFiles.try_emplace(id, std::move(projectFile));
	}
}

void ProjectScreen::switchView(tgui::String id) {
	if (openedFiles.find(focusedFile) != openedFiles.end())
		openedFiles.at(focusedFile)->getView().fileViewFocused = false;
	focusedFile = id;
	fileViewGroup->removeAllWidgets();
	openedFiles.at(id)->addWidgets(fileViewGroup);
	openedFiles.at(id)->getView().fileViewFocused = true;
}

void ProjectScreen::clearView() {
	focusedFile = "";
	fileViewGroup->removeAllWidgets();
	std::unique_ptr<ProjectFile> empty = fileVisitor->visit(EngineFileType::FILE_EMPTY, ".");
	empty->initUi(fileViewGroup);
	empty->addWidgets(fileViewGroup);
}

tgui::Group::Ptr ProjectScreen::createToolBar() {
	auto toolBar = tgui::Group::create({"100%", TOOLBAR_H});
	toolBar->setPosition(0, 0);
	toolBar->getRenderer()->setPadding(8);

	auto barSize = toolBar->getSize().y;

	auto project = Editor::instance->getProject();

	projectLabel = tgui::Label::create("text.");
	projectLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	projectLabel->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	projectLabel->setTextSize(16);
	projectLabel->setPosition({0, 0});
	projectLabel->setSize({tgui::bindWidth(resListWBinder), "100%"});

	assert(project && "project isn't instanciated (nullptr)");

	projectLabel->setText(project->getProgramSettings().projectTitle);

	toolBar->add(projectLabel, "projectLabel");

	auto &fs = Editor::instance->getFs();

	auto playBtnTooltip = Tooltip::create("");
	bindTranslation<Tooltip>(playBtnTooltip, "screen.project.toolbar.play", &Tooltip::setText);
	auto playBtn = tgui::BitmapButton::create();
	auto playtestImg = tgui::Texture(fs.getResourcePath("playtest.png"));
	playBtn->setImage(playtestImg);
	playBtn->setSize({barSize, "100%"});
	playBtn->setPosition({tgui::bindRight(projectLabel), 0});
	playBtn->onPress([] { Editor::instance->getProject()->runProject(); });
	playBtn->setToolTip(playBtnTooltip);
	toolBar->add(playBtn, "playBtn");

	auto buildTooltip = Tooltip::create("");
	bindTranslation<Tooltip>(buildTooltip, "screen.project.toolbar.build", &Tooltip::setText);
	auto buildBtn = tgui::BitmapButton::create();
	auto buildImg = tgui::Texture(fs.getResourcePath("build.png"));
	buildBtn->setImage(buildImg);
	buildBtn->setSize({barSize, "100%"});
	buildBtn->setPosition({tgui::bindRight(playBtn) + 8, 0});
	buildBtn->onPress([project] { project->buildProject(); });
	buildBtn->setToolTip(buildTooltip);
	toolBar->add(buildBtn);

	auto settingsBtn = tgui::BitmapButton::create();
	auto settingsImage = tgui::Texture(fs.getResourcePath("projectsettings.png"));
	settingsBtn->setImage(settingsImage);
	settingsBtn->setSize({barSize, "100%"});
	settingsBtn->setPosition({tgui::bindRight(buildBtn) + 8, 0});
	settingsBtn->onPress([] { Editor::instance->getGui().getChildWindowSubService()->openWindow("project_settings"); });
	toolBar->add(settingsBtn);

	return toolBar;
}

void ProjectScreen::addResourceButtons(EngineFileType fileType) {
	auto &ts = Editor::instance->getTranslations();
	auto project = Editor::instance->getProject();

	this->listedResourcesType = fileType;

	resourcesLayout->removeAllWidgets();
	resourcesLayout->getRenderer()->setSpaceBetweenWidgets(RESLIST_ITEM_PADDING);

	for (auto filePath : project->getPaths(fileType)) {
		std::string fileName = GetFileName(filePath.c_str());

		auto fileBtn = tgui::Button::create(GetFileName(filePath.c_str()));
		fileBtn->setSize("100%", RESLIST_RES_BTN_H);
		fileBtn->onPress([this, fileType, filePath] { addFileView(fileType, filePath); });
		fileBtn->onRightMousePress([this, filePath, project, fileName, &ts] {
			fileContextMenu->getMenuItems().at(0).text = filePath;
			fileContextMenu->setPosition(GetMousePosition().x, GetMousePosition().y);
			fileContextMenu->onMenuItemClick.disconnectAll();
			fileContextMenu->onMenuItemClick(
				[this, filePath, project, fileName, &ts](const std::vector<tgui::String> &hierarchy) {
					if (hierarchy[0] == ts.getKey("context_menu.copy_full_path")) {
						SetClipboardText(filePath.c_str());
					}
					if (hierarchy[0] == ts.getKey("context_menu.delete")) {
						bool allowedDeletion = true;
						auto messageBox = tgui::MessageBox::create();

						if (listedResourcesType == EngineFileType::FILE_MAP) {
							auto defaultRoomPath = project->getGameSettings().defaultLoadingPath;
							if (defaultRoomPath.empty() && project->getPaths(EngineFileType::FILE_MAP).size() <= 1) {
								bindTranslation(messageBox, "dialog.delete_file.room_must_exist",
												&tgui::MessageBox::setText);
								allowedDeletion = false;
							}

							std::string checkedFilePath = TextFormat("maps/%s", fileName.c_str());
							if (defaultRoomPath == checkedFilePath) {
								bindTranslation(messageBox, "dialog.delete_file.room_cannot_be_deleted",
												&tgui::MessageBox::setText);
								allowedDeletion = false;
							}
						}

						if (listedResourcesType == EngineFileType::FILE_ACTOR) {
							auto playerActorPath = project->getGameSettings().playerActorPath;

							if (playerActorPath.empty() && fileName == "playerActor.ractor") {
								bindTranslation(messageBox, "dialog.delete_file.player_actor_must_exist",
												&tgui::MessageBox::setText);
								allowedDeletion = false;
							}

							std::string checkedFilePath = TextFormat("actors/%s", fileName.c_str());
							if (playerActorPath == checkedFilePath) {
								bindTranslation(messageBox, "dialog.delete_file.player_actor_cannot_be_deleted",
												&tgui::MessageBox::setText);
								allowedDeletion = false;
							}
						}

						if (allowedDeletion) {
							messageBox->setText("Are you sure?");
							bindTranslation(messageBox, "dialog.delete_file.title", &tgui::MessageBox::setText);
							messageBox->addButton(ts.getKey("dialog.delete_file.yes"));
							messageBox->addButton(ts.getKey("dialog.delete_file.no"));
						} else {
							messageBox->addButton(ts.getKey("dialog.delete_file.ok"));
						}

						EditorGuiService::centerWidget(messageBox);
						messageBox->setButtonAlignment(tgui::HorizontalAlignment::Right);

						std::weak_ptr<tgui::MessageBox> weakBox = messageBox;

						messageBox->onButtonPress([this, weakBox, filePath, &ts](const tgui::String &button) {
							assert(button == ts.getKey("dialog.delete_file.yes") ||
								   button == ts.getKey("dialog.delete_file.no") ||
								   button == ts.getKey("dialog.delete_file.ok"));

							if (auto box = weakBox.lock()) {
								if (button == ts.getKey("dialog.delete_file.yes")) {
									std::error_code ec;
									std::filesystem::remove(filePath, ec);
									addResourceButtons(listedResourcesType);
									fileTabs->closeTabFilename(GetFileName(filePath.c_str()));
								}

								if (auto parent = box->getParent()) parent->remove(box);
							}
						});

						Editor::instance->getGui().gui->add(messageBox);
					}
				});
			fileContextMenu->openMenu();
		});

		resourcesLayout->add(fileBtn);
	}
}

ResizableContainer::Ptr ProjectScreen::createResourcesList() {
	auto project = Editor::instance->getProject();
	TranslationService &tService = Editor::instance->getTranslations();

	auto group = ResizableContainer::create({modifiable_RESLIST_W, tgui::Layout("100%") - TOOLBAR_H}, {0, TOOLBAR_H});
	group->enableResize(ResizeDirection::RIGHT);
	group->setMinResizeWidth(MIN_RESLIST_W);
	group->setMaxResizeWidth(MAX_RESLIST_W);
	group->useExternalMouseEvent = true;
	group->onResize([this, group] {
		modifiable_RESLIST_W = group->getSize().x;
		layoutReload();
	});

	auto resourceChoose = tgui::ComboBox::create();
	resourceChoose->setPosition(0, 0);
	resourceChoose->setSize("100%", RESLIST_RES_CHOOSE_H);

	int i = 0;
	for (auto typeName : Editor::instance->getFs().getTypeNames()) {
		if (static_cast<EngineFileType>(i) != EngineFileType::FILE_EMPTY) {
			resourceChoose->addItem(typeName);
		}
		i++;
	}
	// resourceChoose->addMultipleItems({"TileSets", "Maps", "Scripts"});
	resourceChoose->setSelectedItem("Maps");
	group->add(resourceChoose);

	auto createResourceBtn = tgui::Button::create();
	bindTranslation<tgui::Button>(createResourceBtn, "screen.project.create_new_resource", &tgui::Button::setText);
	createResourceBtn->setPosition(0, tgui::bindBottom(resourceChoose));
	createResourceBtn->setSize("100%", RESLIST_CREATE_RES_BTN_H);
	createResourceBtn->onPress([this] {
		if (!fileInitVisitor->funcIsEmpty(listedResourcesType)) {
			auto childDialog = NewFileDialog::create();
			childDialog->init(Editor::instance->getGui().gui.get());
			EditorGuiService::centerWidget(childDialog->window);
			fileInitVisitor->visit(listedResourcesType, childDialog);
		}
	});
	group->add(createResourceBtn);

	auto resourceListPanel = tgui::ScrollablePanel::create(
		{"100%", tgui::Layout("100%") - (RESLIST_RES_CHOOSE_H + RESLIST_CREATE_RES_BTN_H)});
	resourceListPanel->setPosition(0, tgui::bindBottom(createResourceBtn));
	resourceListPanel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
	resourceListPanel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

	resourcesLayout = tgui::GrowVerticalLayout::create();
	resourceListPanel->add(resourcesLayout);
	group->add(resourceListPanel);

	resourceChoose->onItemSelect([this](int index) {
		EngineFileType currentFileType = static_cast<EngineFileType>(index);
		addResourceButtons(currentFileType);
	});

	if (project != nullptr) {
		EngineFileType currentFileType = static_cast<EngineFileType>(resourceChoose->getSelectedItemIndex());
		addResourceButtons(currentFileType);
	}

	return group;
}

ProjectFile &ProjectScreen::getCurrentFile() {
	tgui::String currentFile = fileTabs->getSelectedId();
	return *openedFiles.at(currentFile);
}
