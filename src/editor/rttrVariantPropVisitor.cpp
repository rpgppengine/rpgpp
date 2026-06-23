#include "rttrVariantPropVisitor.hpp"

#include <cstdio>

#include "TGUI/String.hpp"
#include "component.hpp"
#include "editor.hpp"
#include "entity.hpp"
#include "gamedata.hpp"
#include "interfaceView.hpp"
#include "raylib.h"
#include "rttr/enumeration.h"
#include "rttr/type.h"
#include "rttr/variant.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/colorField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/nPatchInfoField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"
#include "widgets/propertyFields/uiElementRefField.hpp"

PropertiesBox *VariantPropVisitor::box = nullptr;
InterfaceView *VariantPropVisitor::view = nullptr;
rttr::variant VariantPropVisitor::component = {};

VariantPropVisitor::VariantPropVisitor() {
	map["int"] = p_int;
	map["std::string"] = p_string;
	map["bool"] = p_bool;
	map["Color"] = p_Color;
	map["UIElementRef"] = p_UIElementRef;
	map["FontRef"] = p_FontRef;
	map["ImageRef"] = p_ImageRef;
	map["NPatchInfo"] = p_NPatchInfo;
	map["TextAlignment"] = p_TextAlignment;
	map["CallbacksArray"] = p_CallbacksArray;
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
		box->addSection("Rectangle");
		Rect("Rectangle", component);
		return;
	}

	box->addSection(component.get_type().get_raw_type().get_name().to_string());
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

void VariantPropVisitor::p_bool(rttr::property prop) {
	auto *boolean = prop.get_value(component).get_value<bool *>();

	auto boolField = BoolField::create();
	boolField->label->setText(prop.get_name().to_string());
	boolField->value->setChecked(*boolean);
	boolField->value->onChange([boolean](bool val) { *boolean = val; });
	box->addBooleanField(boolField);
}

void VariantPropVisitor::p_Color(rttr::property prop) {
	auto *color = prop.get_value(component).get_value<struct Color *>();

	rttr::variant componentVar = component;

	auto field = ColorField::create();
	field->label->setText(prop.get_name().to_string());
	field->setColor(*color);
	field->onColorChanged([color, prop, componentVar](struct Color newColor) {
		*color = newColor;
		if (componentVar.is_type<ButtonComponent *>()) {
			componentVar.get_value<ButtonComponent *>()->shownTextColor = newColor;
		}
	});
	box->addColorField(field);
}

void VariantPropVisitor::p_UIElementRef(rttr::property prop) {
	auto &ecs = view->getCoordinator();

	auto *ref = prop.get_value(component).get_value<struct UIElementRef *>();

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
		std::string fullPath =
			TextFormat("%s/fonts/%s", Editor::instance->project->getBasePath().c_str(), newFontName.c_str());
		font->font = LoadFontEx(fullPath.c_str(), 13, nullptr, 256);
	};
	box->addFileField(fontField);

	auto fontSize = IntField::create();
	fontSize->value->setMinimum(8);
	fontSize->value->setMaximum(64);
	fontSize->label->setText(prop.get_name().to_string() + " font size");
	fontSize->value->setValue(font->fontSize);
	fontSize->value->onValueChange([font](int newValue) { font->fontSize = newValue; });
	box->addIntField(fontSize);
}

void VariantPropVisitor::p_ImageRef(rttr::property prop) {
	struct ImageRef *image = prop.get_value(component).get_value<struct ImageRef *>();

	auto imageField = FileField::create();
	imageField->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	imageField->label->setText(prop.get_name().to_string());
	imageField->value->setText(image->path);
	imageField->callback = [image](const tgui::String &path) {
		std::string newImageName = GetFileName(path.toStdString().c_str());
		image->path = newImageName;
		image->texture = LoadTexture(
			TextFormat("%s/images/%s", Editor::instance->project->getBasePath().c_str(), newImageName.c_str()));
	};
	box->addFileField(imageField);

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText(prop.get_name().to_string() + " scale");
	scale->value->setValue(image->scale);
	scale->value->onValueChange([image](int newValue) {
		image->scale = newValue;
		auto loadedImage = LoadImage(TextFormat("images/%s", image->path.c_str()));
		ImageResizeNN(&loadedImage, loadedImage.width * image->scale, loadedImage.height * image->scale);
		image->texture = LoadTextureFromImage(loadedImage);
		UnloadImage(loadedImage);
	});
	box->addIntField(scale);
}

void VariantPropVisitor::p_NPatchInfo(rttr::property prop) {
	struct NPatchInfo *info = prop.get_value(component).get_value<struct NPatchInfo *>();

	rttr::variant var = component;

	auto field = NPatchInfoField::create();
	field->label->setText(prop.get_name().to_string());
	if (var.is_type<NinePatchImageRectComponent *>()) {
		NinePatchImageRectComponent *nPatchComponent = var.get_value<NinePatchImageRectComponent *>();
		field->component = nPatchComponent;
	}

	box->addNPatchFIeld(field);
}

void VariantPropVisitor::p_TextAlignment(rttr::property prop) {
	TextAlignment *alignment = prop.get_value(component).get_value<TextAlignment *>();

	auto field = SelectField::create();
	field->label->setText(prop.get_name().to_string());
	if (TextFindIndex(prop.get_name().to_string().c_str(), "vertical") != -1) {
		field->value->addMultipleItems({"Top", "Center", "Bottom"});
	} else {
		field->value->addMultipleItems({"Left", "Middle", "Right"});
	}
	field->value->setSelectedItemByIndex(*alignment);
	field->value->onItemSelect([alignment](int index) { *alignment = static_cast<TextAlignment>(index); });

	box->addSelectField(field);
}

void VariantPropVisitor::p_CallbacksArray(rttr::property prop) {
	CallbacksArray *arr = prop.get_value(component).get_value<CallbacksArray *>();

	auto enumer = rttr::type::get_by_name("CallbackType").get_enumeration();

	for (int i = 0; i < arr->funcNames.size(); i++) {
		auto field = TextField::create();

		std::string enumValName = enumer.value_to_name(static_cast<CallbackType>(i)).to_string();
		field->label->setText(enumValName);

		field->value->setText(arr->funcNames[i]);
		field->value->onTextChange(
			[i, arr](const tgui::String &newText) { arr->funcNames[i] = newText.toStdString(); });

		box->addTextField(field);
	}
}