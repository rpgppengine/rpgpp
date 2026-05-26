#include "rttrVariantPropVisitor.hpp"

#include <cstdio>

#include "TGUI/String.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "rttr/variant.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/colorField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"
#include "widgets/propertyFields/uiElementRefField.hpp"

PropertiesBox *VariantPropVisitor::box = nullptr;
InterfaceView *VariantPropVisitor::view = nullptr;
rttr::variant VariantPropVisitor::component = {};

VariantPropVisitor::VariantPropVisitor() {
	map["int"] = p_int;
	map["string"] = p_string;
	map["Color"] = p_Color;
	map["UIElementRef"] = p_UIElementRef;
}

void VariantPropVisitor::Rect(std::string name, rttr::variant var) {
	Rectangle *ptr = var.get_value<Rectangle *>();

	auto field = RectangleField::create();
	field->label->setText("Rectangle");
	field->setValue(*ptr);
	field->onChange([ptr](Rectangle newRect) { *ptr = newRect; });
	box->addRectangleField(field);
}

void VariantPropVisitor::componentVisit(rttr::variant component, PropertiesBox *box) {
	this->component = component;
	if (!component.get_type().is_pointer()) {
		return;
	}

	VariantPropVisitor::box = box;

	if (box == nullptr) return;

	if (component.is_type<Rectangle *>()) {
		Rect("Rectangle", component);
		return;
	}

	for (auto &prop : component.get_type().get_properties()) {
		std::string name = prop.get_type().get_raw_type().get_name().to_string();
		if (map[name] != nullptr) {
			map[name](prop);
		}
	}
}

void VariantPropVisitor::p_int(rttr::property prop) {
	auto *propVal = prop.get_value(component).get_value<int *>();

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText(prop.get_name().to_string());
	scale->value->setValue(*propVal);
	scale->value->onValueChange([propVal](int newValue) { *propVal = newValue; });
	box->addIntField(scale);
}

void VariantPropVisitor::p_string(rttr::property prop) {
	auto *string = prop.get_value(component).get_value<std::string *>();

	auto textField = TextField::create();
	textField->label->setText(prop.get_name().to_string());
	textField->value->setText(*string);
	textField->value->onTextChange([string](const tgui::String &newText) { *string = newText.toStdString(); });
	box->addTextField(textField);
}

void VariantPropVisitor::p_Color(rttr::property prop) {
	auto *color = prop.get_value(component).get_value<struct Color *>();

	auto field = ColorField::create();
	field->label->setText(prop.get_name().to_string());
	field->setColor(*color);
	field->onColorChanged([color](struct Color newColor) { *color = newColor; });
	box->addColorField(field);
}

void VariantPropVisitor::p_UIElementRef(rttr::property prop) {
	auto &ecs = view->getCoordinator();

	struct UIElementRef *ref = prop.get_value(component).get_value<struct UIElementRef *>();

	auto field = UIElementRefField::create();
	field->view = view;
	field->ref = ref;
	field->label->setText(prop.get_name().to_string().c_str());
	if (ref->entityId < MAX_ENTITIES) {
		field->value->setText(ecs.getEntityName(ref->entityId));
	}
	box->addRefField(field);
}

void VariantPropVisitor::p_FontRef(rttr::property prop) {
	struct FontRef *font = prop.get_value(component).get_value<struct FontRef *>();

	auto fontField = FileField::create();
	fontField->pathFilters = {{"Font", {"*.ttf"}}};
	fontField->label->setText(prop.get_name().to_string());
	fontField->value->setText(font->path);
	fontField->callback = [font](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		font->path = newFontName;
	};
	box->addFileField(fontField);
}

void VariantPropVisitor::p_ImageRef(rttr::property prop) {
	struct ImageRef *image = prop.get_value(component).get_value<struct ImageRef *>();

	auto imageField = FileField::create();
	imageField->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	imageField->label->setText(prop.get_name().to_string());
	imageField->value->setText(image->path);
	imageField->callback = [image](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		image->path = newFontName;
	};
	box->addFileField(imageField);
}