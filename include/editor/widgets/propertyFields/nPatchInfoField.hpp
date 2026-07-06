#ifndef _RPGPP_NPATCHINFOFIELD_H
#define _RPGPP_NPATCHINFOFIELD_H

#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "raylib.h"

class NPatchInfoField : public tgui::SubwidgetContainer {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;

	typedef std::shared_ptr<NPatchInfoField> Ptr;
	typedef std::shared_ptr<const NPatchInfoField> ConstPtr;

	NPatchInfoField(const char *typeName = "NPatchInfoField", bool initRenderer = true);

	static NPatchInfoField::Ptr create();
	static NPatchInfoField::Ptr create(const tgui::String &label, const tgui::String &value);
	static NPatchInfoField::Ptr copy(NPatchInfoField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;

	NPatchInfo *info = nullptr;
	ImageRef *imageRef = nullptr;
};

#endif
