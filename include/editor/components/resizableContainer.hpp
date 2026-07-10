#ifndef RPGPP_RESIZABLECONTAINER_H
#define RPGPP_RESIZABLECONTAINER_H
#include "TGUI/Layout.hpp"
#include "TGUI/Signal.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widgets/Group.hpp"

enum class ResizeDirectionC { MOVE = 0, LEFT = 1 << 0, RIGHT = 1 << 1, TOP = 1 << 2, BOTTOM = 1 << 3 };

class ResizableContainer : public tgui::Group {
private:
	char resizeFlags = 0;
	int grabberSize = 15;
	int maxResizeWidth = 0;
	int maxResizeHeight = 0;
	int minResizeWidth = 0;
	int minResizeHeight = 0;

	char grabbingFlag = 0;
	tgui::Vector2f startMousePos;
	tgui::Layout2d startSize;
	tgui::Layout2d startPosition;
	bool isGrabbing = false;
	bool cursorModified = false;

	bool inEnabledGrabber(ResizeDirectionC direction, tgui::Vector2f absolutePos);

public:
	ResizableContainer(const tgui::Layout2d &size, const tgui::Layout2d &position);
	void enableResize(ResizeDirectionC direction);
	void disableResize(ResizeDirectionC direction);
	bool isResizable(ResizeDirectionC direction);

	void setMaxResizeWidth(int width);
	void setMaxResizeHeight(int height);
	int getMaxResizeWidth();
	int getMaxResizeHeight();
	void setMinResizeWidth(int width);
	void setMinResizeHeight(int height);
	int getMinResizeWidth();
	int getMinResizeHeight();
	void setGrabberSize(int size);
	int getGrabberSize();

	typedef std::shared_ptr<ResizableContainer> Ptr;
	typedef std::shared_ptr<const ResizableContainer> ConstPtr;

	static ResizableContainer::Ptr create(const tgui::Layout2d &size, const tgui::Layout2d &position);
	static ResizableContainer::Ptr copy(ResizableContainer::ConstPtr widget);

	bool useExternalMouseEvent = false;
	void mouseMoved(tgui::Vector2f pos) override;
	void manualMouseMoved(tgui::Vector2f pos);
	bool leftMousePressed(tgui::Vector2f pos) override;
	void manualLeftMouseReleased(tgui::Vector2f pos);
	void leftMouseReleased(tgui::Vector2f pos) override;

	tgui::SignalTyped<tgui::Layout2d> onResize = {"OnResize"};

protected:
	tgui::Widget::Ptr clone() const override;
};

#endif	// RPGPP_RESIZABLECONTAINER_H
