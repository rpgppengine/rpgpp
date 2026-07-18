#ifndef _RPGPP_SCREENS_PROJECT_SCREEN_H
#define _RPGPP_SCREENS_PROJECT_SCREEN_H

#include <memory>

#include "TGUI/Widgets/ContextMenu.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "components/resizableContainer.hpp"
#include "fileInitVisitor.hpp"
#include "guiScreen.hpp"
#include "projectFile.hpp"
#include "projectFileVisitor.hpp"
#include "services/fileSystemService.hpp"
#include "widgets/fileTab.hpp"

namespace screens {
class ProjectScreen : public UIScreen {
private:
	static const int TOOLBAR_H = 54;

	static const int FILETABS_H = 32;
	static const int MIN_TAB_W_IN_FILETABS = 50;
	static const int MAX_TAB_W_IN_FILETABS = 500;

	int modifiable_RESLIST_W = 264;
	static const int MAX_RESLIST_W = 500;
	static const int MIN_RESLIST_W = 150;
	static const int RESLIST_RES_CHOOSE_H = 32;
	static const int RESLIST_CREATE_RES_BTN_H = 24;
	static const int RESLIST_RES_BTN_H = 36;
	static const int RESLIST_ITEM_PADDING = 4;

	std::unique_ptr<ProjectFileVisitor> fileVisitor;
	std::map<tgui::String, std::unique_ptr<ProjectFile>> openedFiles;
	tgui::Group::Ptr fileViewGroup;
	std::unique_ptr<FileInitVisitor> fileInitVisitor;
	EngineFileType listedResourcesType;

	tgui::Group::Ptr resListWBinder;
	ResizableContainer::Ptr resourcesList;
	tgui::GrowVerticalLayout::Ptr resourcesLayout;
	tgui::ScrollablePanel::Ptr tabsContainer;
	FileTab::Ptr fileTabs;

	tgui::ContextMenu::Ptr fileContextMenu;
	tgui::Label::Ptr projectLabel;

	tgui::String focusedFile;

private:
	void switchView(tgui::String id);
	void clearView();
	tgui::Group::Ptr createToolBar();
	ResizableContainer::Ptr createResourcesList();

public:
	void addFileView(EngineFileType fileType, const std::string &path);
	void addResourceButtons(EngineFileType fileType);
	void mouseMove(int x, int y) override;
	void leftMouseReleased(int x, int y) override;
	void bindMenuBarAndHK(tgui::MenuBar::Ptr menubar) override;
	void layoutReload();
	ProjectFile &getCurrentFile();
	void initItems(tgui::Group::Ptr layout) override;
	const std::string getNameOfScreen() override { return "Project"; }

	tgui::Group::Ptr toolBar;
};
}  // namespace screens
#endif
