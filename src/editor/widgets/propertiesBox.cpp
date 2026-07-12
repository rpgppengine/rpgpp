#include "widgets/propertiesBox.hpp"

#include <raylib.h>

#include <memory>
#include <nlohmann/json.hpp>

#include "TGUI/String.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ChildWindow.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "childWindows/editPropWindow.hpp"
#include "childWindows/newPropWindow.hpp"
#include "editor.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/colorField.hpp"
#include "widgets/propertyFields/fieldConfig.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/interPropField.hpp"
#include "widgets/propertyFields/nPatchInfoField.hpp"
#include "widgets/propertyFields/rectangleField.hpp"
#include "widgets/propertyFields/selectField.hpp"
#include "widgets/propertyFields/textField.hpp"
#include "widgets/propertyFields/uiElementRefField.hpp"

PropertiesBox::PropertiesBox(const char *typeName, bool initRenderer) : tgui::ScrollablePanel(typeName, initRenderer) {
	auto vertLayout = tgui::GrowVerticalLayout::create();
	vertLayout->getRenderer()->setSpaceBetweenWidgets(GAP);

	newPropButton = tgui::Button::create("New Prop..");
	newPropButton->setSize("100%", 24);

	add(vertLayout);
	this->layout = vertLayout;
	getRenderer()->setPadding({0, 8, 0, 8});
}

PropertiesBox::Ptr PropertiesBox::create() { return std::make_shared<PropertiesBox>(); }

PropertiesBox::Ptr PropertiesBox::copy(PropertiesBox::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<PropertiesBox>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr PropertiesBox::clone() const { return std::make_shared<PropertiesBox>(*this); }

void PropertiesBox::draw(tgui::BackendRenderTarget &target, tgui::RenderStates states) const {
	tgui::ScrollablePanel::draw(target, states);
}

void PropertiesBox::addPropsJson(nlohmann::json &j, bool clear, bool editable) {
	if (clear) {
		layout->removeAllWidgets();
	}

	if (editable) {
		if (clear) {
			layout->add(newPropButton);
		}

		// configure the 'New Prop' button
		// newPropButton->setVisible(true);
		layout->add(newPropButton);
		newPropButton->onClick.disconnectAll();
		newPropButton->onClick([this] {
			auto windowPtr = Editor::instance->getGui().getChildWindowSubService()->getWindow("new_prop");
			auto newPropWindow = static_cast<NewPropWindow *>(windowPtr);

			if (interactable != nullptr) {
				newPropWindow->box = this;
				newPropWindow->interactable = interactable;
			}

			Editor::instance->getGui().getChildWindowSubService()->openWindow("new_prop");
		});

		auto windowPtr = Editor::instance->getGui().getChildWindowSubService()->getWindow("edit_prop");
		auto editPropWindow = static_cast<EditPropWindow *>(windowPtr);

		// add fields for the props
		for (auto item : j.items()) {
			auto field = InterPropField::create();
			field->label->setText(item.key());
			field->value->onPress([this, item, editPropWindow] {
				if (interactable != nullptr) {
					editPropWindow->propName = item.key();
					editPropWindow->box = this;
					editPropWindow->interactable = interactable;

					editPropWindow->nameLabel->setText(item.key());

					editPropWindow->open();
				}
			});

			if (item.value().is_string()) {
				field->value->setText("string");
			}
			if (item.value().is_number()) {
				field->value->setText("integer");
			}
			if (item.value().is_boolean()) {
				field->value->setText("boolean");
			}
			if (item.value().is_object()) {
				if (item.value().contains("propType")) {
					std::string propType = item.value().at("propType");

					if (propType == "dialogue") {
						field->value->setText("dialogue");
					}
				}
			}

			addInterPropField(field);
		}
	} else {
		for (auto item : j.items()) {
			printf("%s \n", item.key().c_str());
			if (item.value().is_string()) {
				printf("%s \n", item.value().get<std::string>().c_str());

				auto textField = TextField::create();
				textField->label->setText(item.key());
				textField->value->setText(item.value().get<std::string>());
				textField->value->onTextChange([&j, item, this](const tgui::String &text) {
					std::string st = text.toStdString();
					j.at(item.key()) = st;
					onJsonChanged.emit(this, j);
				});
				addTextField(textField);
			}
			if (item.value().is_number()) {
				auto intField = IntField::create();
				intField->label->setText(item.key());
				intField->value->setValue(item.value().get<float>());
				intField->value->onValueChange([&j, item, this](float value) {
					j.at(item.key()) = value;
					onJsonChanged.emit(this, j);
				});
				addIntField(intField);
			}
			if (item.value().is_boolean()) {
				auto boolField = BoolField::create();
				boolField->label->setText(item.key());
				boolField->value->setChecked(item.value().get<bool>());
				boolField->value->onChange([&j, item, this](bool checked) {
					j.at(item.key()) = checked;
					onJsonChanged.emit(this, j);
				});
				addBooleanField(boolField);
			}
			if (item.value().is_object()) {
				if (item.value().contains("propType")) {
					std::string propType = item.value().at("propType");

					auto fileField = FileField::create();
					fileField->label->setText(item.key());
					fileField->value->setText(item.value().at("value").get<std::string>());
					fileField->callback = [&j, item, this](const tgui::String &filePath) {
						printf("%s \n", filePath.toStdString().c_str());
						printf("%s \n", GetFileNameWithoutExt(filePath.toStdString().c_str()));
						auto &ref = j.at(item.key());

						ref.at("value") = GetFileNameWithoutExt(filePath.toStdString().c_str());

						onJsonChanged.emit(this, j);
					};

					if (propType == "dialogue") {
						fileField->pathFilters = {{"Dialogue", {"*.rdiag"}}};
					}

					addFileField(fileField);
				}
			}
		}
	}
}

tgui::Button::Ptr PropertiesBox::constructButton(const tgui::String &title, std::function<void()> callback) {
	auto button = tgui::Button::create(title);
	button->setSize(TextFormat("100%% - %d", PADDING * 2), 24);
	button->setPosition({PADDING, 0});
	button->onPress(callback);

	layout->add(button);

	return button;
}

void PropertiesBox::addButton(const tgui::String &title, std::function<void()> callback) {
	this->constructButton(title, callback);
}

void PropertiesBox::addIntField(const tgui::String &title, int initialValue, std::function<void(float)> callback) {
	auto group = tgui::Group::create({"100%", 24});

	auto label = tgui::Label::create(title);
	label->setSize("50%", "100%");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Center);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	group->add(label);

	auto value = tgui::SpinControl::create(0.0f, 75.0f);
	value->setSize({"50%", "100%"});
	value->setPosition("50%", 0);
	value->setValue(initialValue);
	value->onValueChange(callback);
	group->add(value);

	layout->add(group);
}

void PropertiesBox::addIntField(IntField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addFileField(FileField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addTextField(TextField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addBooleanField(BoolField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addSelectField(SelectField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addColorField(ColorField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addRefField(UIElementRefField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addNPatchFIeld(NPatchInfoField::Ptr field, const std::string& title) {
	field->setSize({"100%", 24});
	layout->add(field, title);
}

void PropertiesBox::addRectangleField(RectangleField::Ptr field, const std::string& title) {
	field->setSize({"100%", 48});
	layout->add(field, title);
}

void PropertiesBox::addInterPropField(InterPropField::Ptr field) {
	field->setSize({"100%", 24});
	layout->add(field);
}

void PropertiesBox::addSection(const tgui::String &title) {
	auto label = tgui::Label::create(title);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);
	label->setSize({"100%", 24});
	layout->add(label);
}

void PropertiesBox::addPropertiesBox(PropertiesBox::Ptr box) {
	box->setWidth("80%");
	layout->add(box);
}

void PropertiesBox::addWidget(tgui::Widget::Ptr widget) { layout->add(widget); }

tgui::Widget* PropertiesBox::getPropertyField(const std::string& title) {
	return layout->get(title).get();
}

void PropertiesBox::clear() { layout->removeAllWidgets(); }
