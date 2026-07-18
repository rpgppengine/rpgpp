#ifndef _RPGPP_UIELEMENTREFFIELD_H
#define _RPGPP_UIELEMENTREFFIELD_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"

class UIElementRefField : public tgui::SubwidgetContainer {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;

	typedef std::shared_ptr<UIElementRefField> Ptr;
	typedef std::shared_ptr<const UIElementRefField> ConstPtr;

	UIElementRefField(const char *typeName = "UIElementRefField", bool initRenderer = true);

	static UIElementRefField::Ptr create();
	static UIElementRefField::Ptr create(const tgui::String &label, const tgui::String &value);
	static UIElementRefField::Ptr copy(UIElementRefField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;

	InterfaceView *view = nullptr;
	UIElementRef *ref = nullptr;
};

#endif