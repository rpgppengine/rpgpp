#include "rttrVariantPropVisitor.hpp"

#include <cstdio>

#include "TGUI/String.hpp"
#include "component.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
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

VariantPropVisitor::VariantPropVisitor() {
	funcs["Rectangle"] = Rect;
	funcs["InputComponent"] = InputComponent;
	funcs["LabelComponent"] = LabelComponent;
	funcs["TextAreaComponent"] = TextAreaComponent;
	funcs["ColorRectComponent"] = ColorRectComponent;
	funcs["ImageRectComponent"] = ImageRectComponent;
	funcs["NinePatchImageRectComponent"] = NinePatchImageRectComponent;
	funcs["DialogueComponent"] = DialogueComponent;
}

void VariantPropVisitor::visit(std::string name, rttr::variant var, PropertiesBox *box) {
	VariantPropVisitor::box = box;

	if (box == nullptr) return;

	if (funcs[name] != nullptr) {
		funcs[name](name, var);
	}
}

void VariantPropVisitor::Rect(std::string name, rttr::variant var) {
	printf("%s \n", var.get_type().get_name().to_string().c_str());

	Rectangle *ptr = var.get_value<Rectangle *>();

	auto field = RectangleField::create();
	field->label->setText("Rectangle");
	field->setValue(*ptr);
	field->onChange([ptr](Rectangle newRect) { *ptr = newRect; });
	box->addRectangleField(field);
}

void VariantPropVisitor::InputComponent(std::string name, rttr::variant var) {
	auto &ecs = view->getCoordinator();

	struct InputComponent *ptr = var.get_value<struct InputComponent *>();

	auto up = UIElementRefField::create();
	up->view = view;
	up->ref = &ptr->upButton;
	up->label->setText("Up");
	if (ptr->upButton.entityId < MAX_ENTITIES) {
		up->value->setText(ecs.getEntityName(ptr->upButton.entityId));
	}
	box->addRefField(up);

	auto down = UIElementRefField::create();
	down->view = view;
	down->ref = &ptr->downButton;
	down->label->setText("Down");
	if (ptr->downButton.entityId < MAX_ENTITIES) {
		down->value->setText(ecs.getEntityName(ptr->downButton.entityId));
	}
	box->addRefField(down);

	auto left = UIElementRefField::create();
	left->view = view;
	left->ref = &ptr->leftButton;
	left->label->setText("Left");
	if (ptr->leftButton.entityId < MAX_ENTITIES) {
		left->value->setText(ecs.getEntityName(ptr->leftButton.entityId));
	}
	box->addRefField(left);

	auto right = UIElementRefField::create();
	right->view = view;
	right->ref = &ptr->rightButton;
	right->label->setText("Right");
	if (ptr->rightButton.entityId < MAX_ENTITIES) {
		right->value->setText(ecs.getEntityName(ptr->rightButton.entityId));
	}
	box->addRefField(right);
}

void VariantPropVisitor::LabelComponent(std::string name, rttr::variant var) {
	struct LabelComponent *ptr = var.get_value<struct LabelComponent *>();

	auto textField = TextField::create();
	textField->label->setText("Text");
	textField->value->setText(ptr->text);
	textField->value->onTextChange([ptr](const tgui::String &newText) { ptr->text = newText.toStdString(); });
	box->addTextField(textField);

	auto horizontalAlignment = SelectField::create();
	horizontalAlignment->value->addMultipleItems({"Left", "Center", "Right"});
	horizontalAlignment->label->setText("Horizontal Al.");
	horizontalAlignment->value->onItemSelect([ptr](int index) {
		TextAlignment val = static_cast<TextAlignment>(index);
		ptr->horizontalAlignment = val;
	});
	box->addSelectField(horizontalAlignment);

	auto verticalAlignment = SelectField::create();
	verticalAlignment->value->addMultipleItems({"Top", "Middle", "Bottom"});
	verticalAlignment->label->setText("Vertical Al.");
	verticalAlignment->value->onItemSelect([ptr](int index) {
		TextAlignment val = static_cast<TextAlignment>(index);
		ptr->verticalAlignment = val;
	});
	box->addSelectField(verticalAlignment);

	auto fontField = FileField::create();
	fontField->pathFilters = {{"Font", {"*.ttf"}}};
	fontField->label->setText(ptr->fontName);
	fontField->value->setText(ptr->fontName);
	fontField->callback = [ptr](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		ptr->fontName = newFontName;
	};
	box->addFileField(fontField);

	auto fontSize = IntField::create();
	fontSize->value->setMinimum(8);
	fontSize->value->setMaximum(144);
	fontSize->label->setText("Font Size");
	fontSize->value->setValue(ptr->fontSize);
	fontSize->value->onValueChange([ptr](int val) { ptr->fontSize = val; });
	box->addIntField(fontSize);
}

void VariantPropVisitor::TextAreaComponent(std::string name, rttr::variant var) {
	struct TextAreaComponent *ptr = var.get_value<struct TextAreaComponent *>();

	auto textField = TextField::create();
	textField->label->setText("Text");
	textField->value->setText(ptr->text);
	textField->value->onTextChange([ptr](const tgui::String &newText) { ptr->text = newText.toStdString(); });
	box->addTextField(textField);

	auto fontField = FileField::create();
	fontField->pathFilters = {{"Font", {"*.ttf"}}};
	fontField->label->setText(ptr->fontName);
	fontField->value->setText(ptr->fontName);
	fontField->callback = [ptr](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		ptr->fontName = newFontName;
	};
	box->addFileField(fontField);

	auto fontSize = IntField::create();
	fontSize->value->setMinimum(8);
	fontSize->value->setMaximum(144);
	fontSize->label->setText("Font Size");
	fontSize->value->setValue(ptr->fontSize);
	fontSize->value->onValueChange([ptr](int val) { ptr->fontSize = val; });
	box->addIntField(fontSize);
}

void VariantPropVisitor::ColorRectComponent(std::string name, rttr::variant var) {
	struct ColorRectComponent *ptr = var.get_value<struct ColorRectComponent *>();

	auto field = ColorField::create();
	field->label->setText("Color");
	field->setColor(ptr->color);
	field->onColorChanged([ptr](Color color) { ptr->color = color; });
	box->addColorField(field);
}

void VariantPropVisitor::ImageRectComponent(std::string name, rttr::variant var) {
	struct ImageRectComponent *ptr = var.get_value<struct ImageRectComponent *>();

	auto source = FileField::create();
	source->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	source->label->setText("Image");
	source->value->setText(ptr->source);
	source->callback = [ptr](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		ptr->source = newFontName;
	};
	box->addFileField(source);

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText("Scale");
	scale->value->setValue(ptr->scale);
	scale->value->onValueChange([ptr](int val) { ptr->scale = val; });
	box->addIntField(scale);
}

void VariantPropVisitor::NinePatchImageRectComponent(std::string name, rttr::variant var) {
	struct NinePatchImageRectComponent *ptr = var.get_value<struct NinePatchImageRectComponent *>();

	auto source = FileField::create();
	source->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	source->label->setText("Image");
	source->value->setText(ptr->source);
	source->callback = [ptr](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		ptr->source = newFontName;
	};
	box->addFileField(source);

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText("Scale");
	scale->value->setValue(ptr->scale);
	scale->value->onValueChange([ptr](int val) { ptr->scale = val; });
	box->addIntField(scale);
}

void VariantPropVisitor::DialogueComponent(std::string name, rttr::variant var) {
	struct DialogueComponent *ptr = var.get_value<struct DialogueComponent *>();

	auto textField = TextField::create();
	textField->label->setText("Text");
	textField->value->setText(ptr->text);
	textField->value->onTextChange([ptr](const tgui::String &newText) { ptr->text = newText.toStdString(); });
	box->addTextField(textField);
}