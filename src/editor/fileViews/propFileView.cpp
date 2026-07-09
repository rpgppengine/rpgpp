#include "fileViews/propFileView.hpp"

#include "TGUI/String.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "project.hpp"
#include "prop.hpp"
#include "raylib.h"
#include "variant.hpp"
#include "views/propPreview.hpp"
#include "views/propView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/boolField.hpp"
#include "widgets/propertyFields/fileField.hpp"
#include "widgets/propertyFields/selectField.hpp"
PropFileView::PropFileView() {
	TranslationService &ts = Editor::instance->getTranslations();

	propView = PropView::create();
	propView->setSize({TextFormat("100%% - %d", RIGHT_PANEL_W), "100%"});
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(propView));

	auto propBox = PropertiesBox::create();
	propBox->setSize({RIGHT_PANEL_W, "100%"});
	propBox->setPosition({TextFormat("100%% - %d", RIGHT_PANEL_W), 0});

	hasInteractableField = BoolField::create();
	bindTranslation(hasInteractableField->label, "screen.project.propview.has_interactable", &tgui::Label::setText);
	hasInteractableField->value->onChange([this](bool value) {
		propView->getProp()->setHasInteractable(value);
		interactableTypeField->setEnabled(value);
		if (value) {
			interactableTypeField->value->setSelectedItemByIndex(0);
		} else {
			interactableTypeField->value->deselectItem();
		}
		this->dirty = true;
	});
	propBox->addBooleanField(hasInteractableField);

	interactableTypeField = SelectField::create();
	bindTranslation(interactableTypeField->label, "screen.project.propview.interactable_type", &tgui::Label::setText);
	for (const auto &[k, v] : Editor::instance->getProject()->getInteractableNames()) {
		interactableTypeField->value->addItem(GetFileNameWithoutExt(k.c_str()));
	}
	interactableTypeField->value->onItemSelect(
		[this](const tgui::String &item) {
			this->dirty = true;
			propView->getProp()->setInteractableType(item.toStdString());
		});
	propBox->addSelectField(interactableTypeField);

	propImageField = FileField::create();
	bindTranslation(propImageField->label, "screen.project.propview.image", &tgui::Label::setText);
	propImageField->pathFilters = {{"Image", {"*.png"}}};  // TODO: Add more image types
	propImageField->callback = [this](const tgui::String &path) {
		this->dirty = true;
		propView->getProp()->setTextureFromPath(path.toStdString());
	};
	propBox->addFileField(propImageField);

	atlasRectField = RectangleField::create();
	bindTranslation(atlasRectField->label, "screen.project.propview.atlas", &tgui::Label::setText);
	atlasRectField->onChange([this](Rectangle r) {
		this->dirty = true;
		propView->updateAtlasRect(r);
	});
	propBox->addRectangleField(atlasRectField);

	collisionsField = RectangleField::create();
	bindTranslation(collisionsField->label, "screen.project.propview.collision", &tgui::Label::setText);
	collisionsField->onChange([this](Rectangle r) {
		this->dirty = true;
		propView->updateCollisionRect(r);
	});
	propBox->addRectangleField(collisionsField);

	propPreview = PropPreview::create();
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(propPreview));
	propPreview->setSize({"100%", 200});
	propBox->addWidget(propPreview);

	propView->onUpdatedAtlasRect([this](Rectangle r) { atlasRectField->setValue(r); });
	propView->onUpdatedCollisionRect([this](Rectangle r) { collisionsField->setValue(r); });
	widgetContainer.push_back(propBox);
	widgetContainer.push_back(propView);
}

void PropFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;
	if (variant == nullptr) {
		return;
	}

	auto ptr = dynamic_cast<Variant<Prop> *>(variant);
	auto prop = ptr->get();
	if (prop == nullptr) return;

	propView->setProp(prop);
	hasInteractableField->value->setChecked(prop->getHasInteractable());
	propImageField->value->setText(GetFileName(prop->getImagePath()));
	atlasRectField->setValue(prop->getAtlasRect());
	collisionsField->setValue(prop->getCollisionRect());
	interactableTypeField->value->setSelectedItem(prop->getInteractableType());
	interactableTypeField->setEnabled(prop->getHasInteractable());

	propPreview->setProp(prop);
	auto val = propView->getAtlasRect();
	if (val.has_value()) {
		propPreview->setBox(val.value());
	}

	addWidgets(layout);
	this->dirty = false;
}
