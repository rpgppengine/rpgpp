#ifndef _RPGPP_INTERFACEVIEWVIEW_H
#define _RPGPP_INTERFACEVIEWVIEW_H

#include <memory>
#include <string>

#include "TGUI/Signal.hpp"
#include "components/resizableCanvasBox.hpp"
#include "entity.hpp"
#include "interfaceView.hpp"
#include "rttrVariantPropVisitor.hpp"
#include "sol/state.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"

class InterfaceViewView : public WorldView {
private:
	std::unique_ptr<ResizableCanvasBox> canvasBox;
	bool hasActiveElement = false;
	EntityID activeEntity = MAX_ENTITIES;
	void setElementAtMouse();

public:
	typedef std::shared_ptr<InterfaceViewView> Ptr;

	tgui::SignalTyped<std::string> onActiveElementChanged = {"ActiveElementChanged"};
	tgui::SignalTyped<EntityID> onActiveEntityChanged = {"ActiveEntityChanged"};

	InterfaceViewView();

	InterfaceView *ptr{nullptr};
	PropertiesBox *propBox{nullptr};
	VariantPropVisitor *visitor{nullptr};

	void drawCanvas() override;
	void drawOverlay() override;

	static InterfaceViewView::Ptr create();

	void setInterfaceView(InterfaceView *ptr);

	bool leftMousePressed(tgui::Vector2f pos) override;
	void leftMouseReleased(tgui::Vector2f pos) override;
	void mouseMoved(tgui::Vector2f pos) override;

	void selectElement(const std::string &elementName);
};

#endif