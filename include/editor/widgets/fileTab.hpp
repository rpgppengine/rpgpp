#ifndef _RPGPP_FILETAB_H
#define _RPGPP_FILETAB_H

#include <cstddef>
#include <string>

#include "TGUI/Signal.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Tabs.hpp"
#include "components/tooltip.hpp"
#include "fileTabRenderer.hpp"
#include "fileViews/fileView.hpp"

class FileTab : public tgui::Tabs {
private:
	bool isHovering = false;
	bool isHoldingMouse = false;
	bool isDragging = false;
	tgui::Vector2f startMousePos = {0, 0};
	tgui::Vector2f deltaMousePos = {0, 0};
	tgui::Vector2f offsetMousePos = {0, 0};
	int draggedTab = -1;
	int swappedTab = -1;

	Tooltip::Ptr tooltip;

	static const int MARGIN_LR = 8;
	static const int CLOSE_BUTTON_SIZE = 12;
	constexpr static const tgui::Vector2f BUFFER_BEFORE_TAB_MOVE = {10, 10};

	bool cursorModified = false;

	void renderTab(tgui::BackendRenderTarget &target, tgui::RenderStates &states, int idx, float borderWidth,
				   float usableHeight, tgui::Sprite &close) const;
	void closeAndOpenNextTab(std::size_t idx);
	std::map<std::string, FileView *> m_fileViews;

public:
	bool useExternalMouseEvent = false;
	typedef std::shared_ptr<FileTab> Ptr;
	typedef std::shared_ptr<const FileTab> ConstPtr;
	tgui::SignalTyped<tgui::String> onTabClose = {"TabClose"};
	tgui::SignalTyped<tgui::String> onTabSelect = {"TabSelect"};

	FileTab(const char *typeName = "FileTab", bool initRenderer = true);

	static FileTab::Ptr create();

	static FileTab::Ptr copy(FileTab::ConstPtr widget);

	void draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const override;

	FileTabRenderer *getSharedRenderer() override;

	const FileTabRenderer *getSharedRenderer() const override;

	FileTabRenderer *getRenderer() override;

	bool leftMousePressed(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;
	void manualMouseMoved(tgui::Vector2f pos);
	void leftMouseReleased(tgui::Vector2f pos) override;
	void manualLeftMouseReleased(tgui::Vector2f pos);

	bool select(std::size_t i);

	tgui::TabsBase::Tab *getTabId(const std::string id);
	tgui::TabsBase::Tab *getTabName(const std::string fileName);
	size_t addFileTab(const std::string &path, const std::string &fileName, FileView *fileView);
	void closeTabFilename(std::string fileName);
	void closeCurrentTab();

protected:
	Widget::Ptr clone() const override;
};

#endif
