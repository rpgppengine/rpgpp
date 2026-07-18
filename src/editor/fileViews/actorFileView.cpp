#include "fileViews/actorFileView.hpp"

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Label.hpp"
#include "actor.hpp"
#include "bindTranslation.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/translationService.hpp"
#include "views/actorView.hpp"
#include "widgets/frameEditor.hpp"
#include "widgets/propertiesBox.hpp"
#include "widgets/propertyFields/fileField.hpp"

constexpr int IDLE_OFFSET{2};
constexpr int RIGHT_PANEL_W{300};
constexpr int BOTTOM_ANIMATION_PANEL{200};

ActorFileView::ActorFileView() {
	TranslationService &ts = Editor::instance->getTranslations();

	this->actorView = ActorView::create(this);
	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(this->actorView));

	auto canvasHeight = TextFormat("100%% - %d", BOTTOM_ANIMATION_PANEL);
	auto rightPanelOffset = TextFormat("100%% - %d", RIGHT_PANEL_W);

	actorView->setSize({rightPanelOffset, canvasHeight});
	widgetContainer.push_back(this->actorView);

	auto propBox = PropertiesBox::create();
	propBox->setSize({RIGHT_PANEL_W, "100%"});
	propBox->setPosition({rightPanelOffset, 0});

	// The TileSet Editor
	this->tileSetField = FileField::create();
	bindTranslation<tgui::Label>(tileSetField->label, "screen.project.roomview.tileset_file", &tgui::Label::setText);
	tileSetField->pathFilters = {{tileSetField->label->getText(), {"*.rtiles"}}};
	tileSetField->callback = [this](const tgui::String &text) {
		actorView->actor->setTileSet(text.toStdString());
		// fix for the atlas rectangle not sizing up properly.
		actorView->setAtlasRect(actorView->actor->getCurrentAnimationRectangle());
		frameEditor->updateFrameButtons(true);
		actorView->actor->resetAnimation();
		this->dirty = true;
	};
	propBox->addFileField(tileSetField);

	// Collision Rectangle Editor
	collisionField = RectangleField::create();
	bindTranslation<tgui::Label>(collisionField->label, "screen.project.propview.collision", &tgui::Label::setText);
	collisionField->onChange([this](Rectangle r) { actorView->setCollisionRect(r); });
	propBox->addRectangleField(collisionField);

	this->frameEditor = FrameEditor::create(this);
	frameEditor->setPosition({0, canvasHeight});
	frameEditor->getRenderer()->setPadding(5);
	frameEditor->setSize({rightPanelOffset, BOTTOM_ANIMATION_PANEL});
	frameEditor->onAttributeChanged.connect([this]() {
		this->dirty = true;
	});

	widgetContainer.push_back(frameEditor);
	widgetContainer.push_back(propBox);
}

void ActorFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Actor> *>(variant);
		const auto ptrRaw = ptr->get();

		this->actorView->setActor(ptrRaw);

		this->tileSetField->setValue(GetFileName(ptrRaw->getTileSetSource().c_str()));
		this->collisionField->setValue(ptrRaw->getCollisionRect());

		actorView->actor->onFrameChanged = [this](const int &frame) {
			this->frameEditor->onFrameChange(frame);
			this->actorView->setAtlasRect(actorView->actor->getCurrentAnimationRectangle());
		};

		actorView->onAttributeChanged.connect([this]{
			this->dirty = true;
		});

		// NOTE: Always initialize this later. Otherwise, we might see
		// flying ComboBoxes, John.
		frameEditor->init();

		addWidgets(layout);
		this->dirty = false;
	}
}
