#include "fileViews/interactableFileView.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "bindTranslation.hpp"
#include "interactable.hpp"
#include "raylib.h"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/textField.hpp"

InteractableFileView::InteractableFileView() {
	auto props = PropertiesBox::create();
	props->setSize({RIGHT_PANEL_W, "100%"});
	props->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	nameField = TextField::create();
	bindTranslation<tgui::Label>(nameField->label, "screen.project.interactableview.display_name",
								 &tgui::Label::setText);
	nameField->value->onTextChange([this](const tgui::String &text) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setDisplayTitle(text.toStdString());
			this->dirty = true;
		}
	});
	props->addTextField(nameField);

	scriptField = FileField::create();
	scriptField->pathFilters = {{"Script", {"*.lua"}}};
	bindTranslation<tgui::Label>(scriptField->label, "screen.project.interactableview.script", &tgui::Label::setText);
	scriptField->callback = [this](const tgui::String &path) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setScriptSourcePath(TextFormat("scripts/%s", GetFileName(path.toStdString().c_str())));
			this->dirty = true;
		}
	};
	props->addFileField(scriptField);

	onTouchField = BoolField::create();
	bindTranslation<tgui::Label>(onTouchField->label, "screen.project.interactableview.on_touch",
								 &tgui::Label::setText);
	onTouchField->value->onChange([this](bool value) {
		if (variant != nullptr) {
			const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
			const auto interactable = ptr->get();

			interactable->setOnTouch(value);
			this->dirty = true;
		}
	});
	props->addBooleanField(onTouchField);

	interProps = PropertiesBox::create();

	props->addPropertiesBox(interProps);

	widgetContainer.push_back(props);
}

void InteractableFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Interactable> *>(variant);
		const auto interactable = ptr->get();

		nameField->value->setText(interactable->getDisplayTitle());
		scriptField->value->setText(GetFileName(interactable->getScriptSourcePath().c_str()));
		onTouchField->value->setChecked(interactable->isOnTouch());

		interProps->interactable = interactable;
		interProps->addPropsJson(interactable->getProps(), true, true);

		addWidgets(layout);
	}
}
