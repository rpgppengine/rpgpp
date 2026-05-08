#ifndef _RPGPP_INTERFACEVIEWVIEW_H
#define _RPGPP_INTERFACEVIEWVIEW_H

#include <memory>
#include <string>

#include "TGUI/Signal.hpp"
#include "components/resizableCanvasBox.hpp"
#include "interfaceView.hpp"
#include "uiElement.hpp"
#include "views/worldView.hpp"

class InterfaceViewView : public WorldView {
private:
	std::unique_ptr<ResizableCanvasBox> canvasBox;
	bool hasActiveElement = false;
	UIElement *activeElement = nullptr;
	void setElementAtMouse();

public:
	typedef std::shared_ptr<InterfaceViewView> Ptr;

	tgui::SignalTyped<std::string> onActiveElementChanged = {"ActiveElementChanged"};

	InterfaceViewView();

	InterfaceView *ptr{nullptr};

	void drawCanvas() override;
	void drawOverlay() override;

	static InterfaceViewView::Ptr create();

	void setInterfaceView(InterfaceView *ptr);

	bool leftMousePressed(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;

	void selectElement(const std::string &elementName);
	UIElement *getActiveElement();
};

#endif