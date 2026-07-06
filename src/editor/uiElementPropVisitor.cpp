#include "uiElementPropVisitor.hpp"

#include "editor.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/colorField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/nPatchInfoField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"
#include "widgets/propertyFields/uiElementRefField.hpp"

void UIElementPropVisitor::operator()(int val) {
	std::string k = key;

	auto field = IntField::create();
	field->value->setMinimum(1);
	field->value->setMaximum(5);
	field->label->setText(key);
	field->value->setValue(val);
	field->value->onValueChange([this, k](int newValue) { element->props[k] = newValue; });
	box->addIntField(field);
}

void UIElementPropVisitor::operator()(bool val) {
	std::string k = key;

	auto field = BoolField::create();
	field->label->setText(key);
	field->value->setChecked(val);
	field->value->onChange([this, k](bool newValue) { element->props[k] = newValue; });
	box->addBooleanField(field);
}

void UIElementPropVisitor::operator()(const std::string &val) {
	std::string k = key;

	auto textField = TextField::create();
	textField->label->setText(key);
	textField->value->setText(val);
	textField->value->onTextChange(
		[this, k](const tgui::String &newText) { element->props[k] = newText.toStdString(); });
	box->addTextField(textField);
}

void UIElementPropVisitor::operator()(Rectangle val) {
	std::string k = key;

	auto field = RectangleField::create();
	field->label->setText("Rectangle");
	field->setValue(val);
	field->onChange([this, k](Rectangle newRect) { element->props[k] = newRect; });
	box->addRectangleField(field);
}

void UIElementPropVisitor::operator()(Color val) {
	std::string k = key;

	auto field = ColorField::create();
	field->label->setText(key);
	field->setColor(val);
	field->onColorChanged([this, k](Color newValue) { element->props[k] = newValue; });
	box->addColorField(field);
}

void UIElementPropVisitor::operator()(ImageRef val) {
	std::string k = key;

	auto imageField = FileField::create();
	imageField->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	imageField->label->setText(key);
	imageField->value->setText(val.path);
	imageField->callback = [this, k](const tgui::String &path) {
		std::string newImageName = GetFileName(path.toStdString().c_str());

		auto &image = std::get<ImageRef>(element->props[k]);
		image.path = newImageName;
		image.texture = LoadTexture(
			TextFormat("%s/images/%s", Editor::instance->project->getBasePath().c_str(), newImageName.c_str()));
	};
	box->addFileField(imageField);

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText(k + " scale");
	scale->value->setValue(val.scale);
	scale->value->onValueChange([this, k](int newValue) {
		auto &image = std::get<ImageRef>(element->props[k]);

		image.scale = newValue;
		auto loadedImage = LoadImage(TextFormat("images/%s", image.path.c_str()));
		ImageResizeNN(&loadedImage, loadedImage.width * image.scale, loadedImage.height * image.scale);
		image.texture = LoadTextureFromImage(loadedImage);
		UnloadImage(loadedImage);
	});
	box->addIntField(scale);
}

void UIElementPropVisitor::operator()(FontRef val) {
	std::string k = key;

	auto fontField = FileField::create();
	fontField->pathFilters = {{"Font", {"*.ttf"}}};
	fontField->label->setText(key);
	fontField->value->setText(val.path);
	fontField->callback = [this, k](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());
		std::string fullPath =
			TextFormat("%s/fonts/%s", Editor::instance->project->getBasePath().c_str(), newFontName.c_str());

		auto &font = std::get<FontRef>(element->props[k]);
		font.path = newFontName;
		font.font = LoadFontEx(fullPath.c_str(), 13, nullptr, 256);
	};
	box->addFileField(fontField);

	auto fontSize = IntField::create();
	fontSize->value->setMinimum(8);
	fontSize->value->setMaximum(64);
	fontSize->label->setText(k + " font size");
	fontSize->value->setValue(val.fontSize);
	fontSize->value->onValueChange([this, k](int newValue) {
		auto &font = std::get<FontRef>(element->props[k]);
		font.fontSize = newValue;
	});
	box->addIntField(fontSize);
}

void UIElementPropVisitor::operator()(UIElementRef val) {
	std::string k = key;
	auto ref = std::get_if<UIElementRef>(&element->props[k]);

	uiElementRef(ref, k);
}

void UIElementPropVisitor::operator()(VerticalAlignment val) {
	std::string k = key;

	auto field = SelectField::create();
	field->label->setText(key);
	field->value->addMultipleItems({"Top", "Center", "Bottom"});
	field->value->setSelectedItemByIndex(val.val);
	field->value->onItemSelect([this, k](int index) {
		auto &alignment = std::get<VerticalAlignment>(element->props[k]);
		alignment.val = static_cast<TextAlignment>(index);
	});

	box->addSelectField(field);
}

void UIElementPropVisitor::operator()(HorizontalAlignment val) {
	std::string k = key;

	auto field = SelectField::create();
	field->label->setText(key);
	field->value->addMultipleItems({"Left", "Middle", "Right"});
	field->value->setSelectedItemByIndex(val.val);
	field->value->onItemSelect([this, k](int index) {
		auto &alignment = std::get<HorizontalAlignment>(element->props[k]);
		alignment.val = static_cast<TextAlignment>(index);
	});

	box->addSelectField(field);
}

void UIElementPropVisitor::uiElementRef(UIElementRef *ref, const std::string &key) {
	auto field = UIElementRefField::create();
	field->view = view;
	field->ref = ref;
	field->label->setText(key);
	if (ref->entityId < MAX_ELEMENTS) {
		field->value->setText(view->getEntityName(ref->entityId));
	}
	box->addRefField(field);
}

void UIElementPropVisitor::operator()(NPatchInfo val) {
	std::string k = key;

	auto npatchInfo = std::get_if<NPatchInfo>(&element->props[k]);
	auto imageRef = std::get_if<ImageRef>(&element->props["image"]);

	auto field = NPatchInfoField::create();
	field->label->setText(k);
	field->info = npatchInfo;
	field->imageRef = imageRef;

	box->addNPatchFIeld(field);
}

void UIElementPropVisitor::operator()(InputC val) {
	std::string k = key;

	auto input = std::get_if<InputC>(&element->props[k]);
	uiElementRef(&input->upButton, "upButton");
	uiElementRef(&input->downButton, "downButton");
	uiElementRef(&input->leftButton, "leftButton");
	uiElementRef(&input->rightButton, "rightButton");

	for (int i = 0; i < input->funcNames.funcNames.size(); i++) {
		auto field = TextField::create();

		std::string enumValName = std::to_string(i);
		field->label->setText(enumValName);

		field->value->setText(input->funcNames.funcNames[i]);
		field->value->onTextChange(
			[i, input](const tgui::String &newText) { input->funcNames.funcNames[i] = newText.toStdString(); });

		box->addTextField(field);
	}
}
