#ifndef _RPGPP_PROPERTIESBOX_H
#define _RPGPP_PROPERTIESBOX_H

#include <functional>
#include <nlohmann/json_fwd.hpp>

#include "TGUI/Signal.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "any_ptr/any_ptr.h"
#include "interactable.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/interPropField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"

class PropertiesBox : public tgui::ChildWindow {
protected:
	static const int GAP = 4;

	tgui::Button::Ptr newPropButton;
	tgui::GrowVerticalLayout::Ptr layout;
	tgui::Widget::Ptr clone() const override;

public:
	typedef std::shared_ptr<PropertiesBox> Ptr;
	typedef std::shared_ptr<const PropertiesBox> ConstPtr;

	tgui::SignalTyped<nlohmann::json> onJsonChanged = {"JsonChanged"};

	Interactable *interactable = nullptr;

	std::map<std::string, xxx::any_ptr> anyProps = {};

	PropertiesBox(const char *typeName = "PropertiesBox", bool initRenderer = true);

	static PropertiesBox::Ptr create();
	static PropertiesBox::Ptr copy(PropertiesBox::ConstPtr widget);

	void draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const override;

	void addPropsJson(nlohmann::json &j, bool clear = true, bool editable = false);
	void addIntField(const tgui::String &title, int initialValue, std::function<void(float)> callback);
	void addIntField(IntField::Ptr field);
	void addFileField(FileField::Ptr field);
	void addTextField(TextField::Ptr field);
	void addBooleanField(BoolField::Ptr field);
	void addRectangleField(RectangleField::Ptr field);
	void addSelectField(SelectField::Ptr field);
	void addButton(const tgui::String &title, std::function<void()> callback);
	void addInterPropField(InterPropField::Ptr field);
	void addPropertiesBox(PropertiesBox::Ptr box);
	void addWidget(tgui::Widget::Ptr widget);
	tgui::Button::Ptr constructButton(const tgui::String &title, std::function<void()> callback);
};

#endif
