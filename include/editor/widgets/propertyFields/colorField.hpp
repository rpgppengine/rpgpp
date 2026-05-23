#ifndef _RPGPP_COLORFIELD_H
#define _RPGPP_COLORFIELD_H

#include "TGUI/Color.hpp"
#include "TGUI/Signal.hpp"
#include "TGUI/SubwidgetContainer.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ColorPicker.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "raylib.h"

class ColorField : public tgui::Group {
private:
	void updateSize();
	tgui::Color color = {255, 0, 0};

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::Button::Ptr value;
	tgui::ColorPicker::Ptr panel;

	tgui::SignalTyped<Color> onColorChanged = {"ColorChanged"};

	typedef std::shared_ptr<ColorField> Ptr;
	typedef std::shared_ptr<const ColorField> ConstPtr;

	ColorField(const char *typeName = "ColorField", bool initRenderer = true);

	static ColorField::Ptr create();
	static ColorField::Ptr create(const tgui::String &label, const tgui::String &value);
	static ColorField::Ptr copy(ColorField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;

	void setColor(Color color);
	void setColor(tgui::Color color);
};

#endif