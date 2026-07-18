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

void UIElementPropVisitor::visitProps() {
	auto fileType = screen->getCurrentFile().getFileType();
	if (fileType == EngineFileType::FILE_INTERFACEVIEW) {
		if (element == nullptr) return;

		auto scrollbar = box->getVerticalScrollbar()->getValue();
		box->clear();
		for (auto &[title, variant] : element->props) {
			key = title;
			std::visit(*this, variant);
		}
		box->getVerticalScrollbar()->setValue(scrollbar);
	}
}

void UIElementPropVisitor::operator()(int val) {
	std::string k = key;
	int oldValue = val;
	int currentId = currentItemId;

	auto field = IntField::create();
	field->value->setMinimum(0);
	field->value->setMaximum(1000);
	field->label->setText(key);
	field->value->setValue(val);
	field->value->onValueChange([this, k, oldValue, currentId, field](int newValue) {
		printf("%f \n", field->value->getValue());
		printf("%i \n", newValue);

		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			element->props[k] = newValue;
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			element->props[k] = oldValue;
			element->config();
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addIntField(field);
}

void UIElementPropVisitor::operator()(bool val) {
	std::string k = key;
	bool oldValue = val;
	int currentId = currentItemId;

	auto field = BoolField::create();
	field->label->setText(key);
	field->value->setChecked(val);
	field->value->onChange([this, k, oldValue, currentId](bool newValue) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			element->props[k] = newValue;
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			element->props[k] = oldValue;
			element->config();
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addBooleanField(field, k);
}

void UIElementPropVisitor::operator()(const std::string &val) {
	std::string k = key;
	std::string oldValue = val;
	int currentId = this->currentItemId;

	auto textField = TextField::create();
	textField->label->setText(key);
	textField->value->setText(val);
	textField->value->onTextChange([this, k, currentId, oldValue](const tgui::String &newText) {
		auto newValue = newText.toStdString();

		element->props[k] = newValue;
		element->config();

		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			element->props[k] = newValue;
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			element->props[k] = oldValue;
			element->config();
			visitProps();
		};
		action->executeOnAdd = false;
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addTextField(textField, k);
}

void UIElementPropVisitor::operator()(Rectangle val) {
	std::string k = key;
	Rectangle oldValue = val;
	int currentId = this->currentItemId;

	auto field = RectangleField::create();
	field->label->setText(k);
	field->setValue(val);
	field->onChange([this, k, currentId, oldValue](Rectangle newValue) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			element->props[k] = newValue;
			element->config();
			//visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			element->props[k] = oldValue;
			element->config();
			//visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addRectangleField(field, k);
}

void UIElementPropVisitor::operator()(Color val) {
	std::string k = key;
	Color oldValue = val;
	int currentId = this->currentItemId;

	auto field = ColorField::create();
	field->label->setText(key);
	field->setColor(val);
	field->onColorChanged([this, k, currentId, oldValue](Color newValue) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			element->props[k] = newValue;
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			element->props[k] = oldValue;
			element->config();
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addColorField(field, k);
}

void UIElementPropVisitor::operator()(ImageRef val) {
	std::string k = key;
	int currentId = this->currentItemId;

	std::string oldImageName = val.path;

	auto imageField = FileField::create();
	imageField->pathFilters = {{"Image", {"*.png", "*.jpg"}}};
	imageField->label->setText(key);
	imageField->value->setText(val.path);
	imageField->callback = [this, k, currentId, oldImageName](const tgui::String &path) {
		std::string newImageName = GetFileName(path.toStdString().c_str());

		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newImageName, currentId] {
			auto &image = std::get<ImageRef>(element->props[k]);
			image.path = newImageName;
			image.texture = LoadTexture(
				TextFormat("%s/images/%s", Editor::instance->project->getBasePath().c_str(), newImageName.c_str()));
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldImageName, currentId] {
			auto &image = std::get<ImageRef>(element->props[k]);
			image.path = oldImageName;
			image.texture = LoadTexture(
				TextFormat("%s/images/%s", Editor::instance->project->getBasePath().c_str(), oldImageName.c_str()));
			element->config();
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	};
	box->addFileField(imageField, k);

	int oldScale = val.scale;

	auto scale = IntField::create();
	scale->value->setMinimum(1);
	scale->value->setMaximum(5);
	scale->label->setText(k + " scale");
	scale->value->setValue(val.scale);
	scale->value->onValueChange([this, k, currentId, oldScale](int newScale) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newScale, currentId] {
			auto &image = std::get<ImageRef>(element->props[k]);
			image.scaleImage(newScale);

			element->config();

			visitProps();
		};
		action->onUndo = [this, k, oldScale, currentId] {
			auto &image = std::get<ImageRef>(element->props[k]);
			image.scaleImage(oldScale);
			element->config();

			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});
	box->addIntField(scale, k + " scale");
}

void UIElementPropVisitor::operator()(FontRef val) {
	std::string k = key;
	int currentId = this->currentItemId;

	std::string oldFontName = val.path;

	auto fontField = FileField::create();
	fontField->pathFilters = {{"Font", {"*.ttf"}}};
	fontField->label->setText(key);
	fontField->value->setText(val.path);
	fontField->callback = [this, k, currentId, oldFontName](const tgui::String &path) {
		std::string newFontName = GetFileNameWithoutExt(path.toStdString().c_str());

		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newFontName, currentId] {
			std::string fullPath =
				TextFormat("%s/fonts/%s", Editor::instance->project->getBasePath().c_str(), newFontName.c_str());

			auto &font = std::get<FontRef>(element->props[k]);
			font.path = newFontName;
			font.font = LoadFontEx(fullPath.c_str(), 13, nullptr, 256);
			element->config();

			visitProps();
		};
		action->onUndo = [this, k, oldFontName, currentId] {
			std::string fullPath =
				TextFormat("%s/fonts/%s", Editor::instance->project->getBasePath().c_str(), oldFontName.c_str());

			auto &font = std::get<FontRef>(element->props[k]);
			font.path = oldFontName;
			font.font = LoadFontEx(fullPath.c_str(), 13, nullptr, 256);
			element->config();

			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	};
	box->addFileField(fontField, k);

	int oldFontSize = val.fontSize;

	auto fontSize = IntField::create();
	fontSize->value->setMinimum(8);
	fontSize->value->setMaximum(64);
	fontSize->label->setText(k + " font size");
	fontSize->value->setValue(val.fontSize);
	fontSize->value->onValueChange([this, k, currentId, oldFontSize](int newFontSize) {
		auto &font = std::get<FontRef>(element->props[k]);
		font.fontSize = newFontSize;

		element->config();

		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newFontSize, currentId] {
			auto &font = std::get<FontRef>(element->props[k]);
			font.fontSize = newFontSize;
			element->config();
			visitProps();
		};
		action->onUndo = [this, k, oldFontSize, currentId] {
			auto &font = std::get<FontRef>(element->props[k]);
			font.fontSize = oldFontSize;
			element->config();
			visitProps();
		};
	});
	box->addIntField(fontSize, k + " font size");
}

void UIElementPropVisitor::operator()(UIElementRef val) {
	std::string k = key;
	auto ref = std::get_if<UIElementRef>(&element->props[k]);

	uiElementRef(ref, k);
}

void UIElementPropVisitor::operator()(VerticalAlignment val) {
	std::string k = key;
	int currentId = this->currentItemId;
	auto oldValue = val.val;

	auto field = SelectField::create();
	field->label->setText(key);
	field->value->addMultipleItems({"Top", "Center", "Bottom"});
	field->value->setSelectedItemByIndex(val.val);
	field->value->onItemSelect([this, k, currentId, oldValue](int newValue) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			auto &alignment = std::get<VerticalAlignment>(element->props[k]);
			alignment.val = static_cast<TextAlignment>(newValue);
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			auto &alignment = std::get<VerticalAlignment>(element->props[k]);
			alignment.val = static_cast<TextAlignment>(oldValue);
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});

	box->addSelectField(field, k);
}

void UIElementPropVisitor::operator()(HorizontalAlignment val) {
	std::string k = key;
	int currentId = this->currentItemId;
	auto oldValue = val.val;

	auto field = SelectField::create();
	field->label->setText(key);
	field->value->addMultipleItems({"Left", "Middle", "Right"});
	field->value->setSelectedItemByIndex(val.val);
	field->value->onItemSelect([this, k, currentId, oldValue](int newValue) {
		auto action = std::make_unique<Action>();
		action->onAction = [this, k, newValue, currentId] {
			auto &alignment = std::get<HorizontalAlignment>(element->props[k]);
			alignment.val = static_cast<TextAlignment>(newValue);
			visitProps();
		};
		action->onUndo = [this, k, oldValue, currentId] {
			auto &alignment = std::get<HorizontalAlignment>(element->props[k]);
			alignment.val = static_cast<TextAlignment>(oldValue);
			visitProps();
		};
		screen->getCurrentFile().getView().pushAction(std::move(action));
	});

	box->addSelectField(field, k);
}

void UIElementPropVisitor::uiElementRef(UIElementRef *ref, const std::string &key) {
	auto field = UIElementRefField::create();
	field->view = view;
	field->ref = ref;
	field->label->setText(key);
	if (ref->entityId < MAX_ELEMENTS) {
		field->value->setText(view->getEntityName(ref->entityId));
	}
	if (!ref->title.empty() && view->findByName(ref->title) < MAX_ELEMENTS) {
		field->value->setText(ref->title);
	}
	box->addRefField(field, key);
}

void UIElementPropVisitor::operator()(NPatchInfo val) {
	std::string k = key;

	auto npatchInfo = std::get_if<NPatchInfo>(&element->props[k]);
	auto imageRef = std::get_if<ImageRef>(&element->props["image"]);

	auto field = NPatchInfoField::create();
	field->label->setText(k);
	field->info = npatchInfo;
	field->imageRef = imageRef;

	box->addNPatchFIeld(field, k);
}

void UIElementPropVisitor::operator()(StringVector val) {
	std::string k = key;

	StringVector *vec = std::get_if<StringVector>(&element->props[k]);

	auto field = ListField<std::string>::create();
	field->label->setText(k);
	field->value->onPress([this, vec, field] {
		editListFieldWindow->field = field.get();
		editListFieldWindow->setup(vec);
		editListFieldWindow->open();
	});

	box->addListField<std::string>(field);
}

void UIElementPropVisitor::operator()(InputC val) {
	std::string k = key;

	auto input = std::get_if<InputC>(&element->props[k]);
	uiElementRef(&input->upButton, "upButton");
	uiElementRef(&input->downButton, "downButton");
	uiElementRef(&input->leftButton, "leftButton");
	uiElementRef(&input->rightButton, "rightButton");

	for (auto& funcName : input->funcNames.funcNames) {
		auto field = TextField::create();

		field->label->setText(funcName.first);

		field->value->setText(funcName.second);
		field->value->onTextChange(
			[funcName, input](const tgui::String &newText) { input->funcNames.funcNames[funcName.first] = newText.toStdString(); });

		box->addTextField(field, k);
	}
}
