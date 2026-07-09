#ifndef _RPGPP_EDITLISTFIELDWINDOW_H
#define _RPGPP_EDITLISTFIELDWINDOW_H

#include <vector>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/ScrollablePanel.hpp"
#include "bindTranslation.hpp"
#include "childWindows/popupWindow.hpp"
#include "listHelper.hpp"
#include "nlohmann/json_fwd.hpp"
#include "raylib.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/intField.hpp"
#include "widgets/propertyFields/listField.hpp"
#include "widgets/propertyFields/textField.hpp"

template <typename T>
class EditListFieldWindow : public PopupWindow {
public:
	tgui::GrowVerticalLayout::Ptr layout;
	std::vector<T> *list;
	ListField<T> *field;
	EditListFieldWindow() : PopupWindow("Edit List..") {
		bindTranslation(this->currentWindow, "dialog.edit_list_field.title", &tgui::ChildWindow::setTitle);
		list = nullptr;
		currentWindow->setSize(280, 180);

		auto panel = tgui::Panel::create();
		panel->getRenderer()->setPadding({4, 4});

		auto addButton = tgui::Button::create();
		bindTranslation<tgui::Button>(addButton, "dialog.edit_list_field.add", &tgui::Button::setText);
		addButton->setPosition(0, 0);
		addButton->setSize("100%", "10%");
		addButton->onPress([this] {
			addItem();
			field->value->setText(VecToString(*list));
		});
		panel->add(addButton);

		auto scrollablePanel = tgui::ScrollablePanel::create({"100%", "80%"});
		scrollablePanel->setPosition(0, "10%");

		layout = tgui::GrowVerticalLayout::create();
		layout->getRenderer()->setSpaceBetweenWidgets(4);
		scrollablePanel->add(layout);

		auto closeButton = tgui::Button::create();
		bindTranslation<tgui::Button>(closeButton, "dialog.edit_list_field.close", &tgui::Button::setText);
		closeButton->setText("Close");
		closeButton->setPosition(0, "90%");
		closeButton->setSize("100%", "10%");
		closeButton->onClick([this] { close(); });
		panel->add(closeButton);

		panel->add(scrollablePanel);
		currentWindow->add(panel);
	}

	void addItem(int index = -1);

	void setup(std::vector<T> *listPtr) {
		this->list = listPtr;
		this->layout->removeAllWidgets();
		int i = 0;
		for (auto &item : *list) {
			addItem(i);
			i++;
		}
	}
};

template <>
inline void EditListFieldWindow<std::string>::addItem(int index) {
	if (index == -1) {
		auto &newItem = list->emplace_back();
		auto newField = TextField::create();
		newField->setSize({"100%", 24});
		newField->label->setText(TextFormat("[%lu]", list->size() - 1));
		newField->value->onTextChange([&newItem, this](const tgui::String &text) {
			newItem = text.toStdString();
			field->value->setText(VecToString(*list));
		});

		newField->enableRemoving();
		newField->remove->onClick([this, &newField, index] {
			list->erase(list->begin() + index);
			layout->remove(newField);
			field->value->setText(VecToString(*list));
		});

		layout->add(newField);
	} else {
		auto &newItem = list->at(index);
		auto newField = TextField::create();
		newField->setSize({"100%", 24});
		newField->label->setText(TextFormat("[%i]", index));
		newField->value->setText(newItem);
		newField->value->onTextChange([&newItem, this](const tgui::String &text) {
			newItem = text.toStdString();
			field->value->setText(VecToString(*list));
		});

		newField->enableRemoving();
		newField->remove->onClick([this, &newField, index] {
			list->erase(list->begin() + index);
			setup(list);
			field->value->setText(VecToString(*list));
		});

		layout->add(newField);
	}
}

template <>
inline void EditListFieldWindow<int>::addItem(int index) {
	if (index == -1) {
		auto &newItem = list->emplace_back();
		index = list->size() - 1;

		auto newField = IntField::create();
		newField->setSize({"100%", 24});
		newField->label->setText(TextFormat("[%i]", index));
		newField->value->setMinimum(0);
		newField->value->setMaximum(100);
		newField->value->onValueChange([&newItem, this](int value) {
			newItem = value;
			field->value->setText(VecToString(*list));
		});
		newField->value->setValue(newItem);
		newField->enableRemoving();
		newField->remove->onClick([this, &newField, index] {
			list->erase(list->begin() + index);
			layout->remove(newField);
			field->value->setText(VecToString(*list));
		});

		layout->add(newField);
	} else {
		auto &newItem = list->at(index);

		auto newField = IntField::create();
		newField->setSize({"100%", 24});
		newField->label->setText(TextFormat("[%i]", index));
		newField->value->setMinimum(0);
		newField->value->setMaximum(100);
		newField->value->onValueChange([&newItem, this](int value) {
			newItem = value;
			field->value->setText(VecToString(*list));
		});
		newField->value->setValue(newItem);
		newField->enableRemoving();
		newField->remove->onClick([this, &newField, index] {
			list->erase(list->begin() + index);
			setup(list);
			field->value->setText(VecToString(*list));
		});

		layout->add(newField);
	}
}

#endif
