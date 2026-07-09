#ifndef _RPGPP_TEXTFIELD_H
#define _RPGPP_TEXTFIELD_H

#include <TGUI/SubwidgetContainer.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <TGUI/Widgets/Label.hpp>

#include "TGUI/Widgets/BitmapButton.hpp"

class TextField : public tgui::SubwidgetContainer {
private:
	void updateSize();

protected:
	tgui::Widget::Ptr clone() const override;

public:
	tgui::Label::Ptr label;
	tgui::EditBox::Ptr value;
	tgui::BitmapButton::Ptr remove;
	bool removable = false;

	typedef std::shared_ptr<TextField> Ptr;
	typedef std::shared_ptr<const TextField> ConstPtr;

	TextField(const char *typeName = "TextField", bool initRenderer = true);

	static TextField::Ptr create();
	static TextField::Ptr copy(TextField::ConstPtr widget);

	void setSize(const tgui::Layout2d &size) override;

	void enableRemoving();
};

#endif
