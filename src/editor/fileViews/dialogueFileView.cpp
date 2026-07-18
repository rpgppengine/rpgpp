#include "fileViews/dialogueFileView.hpp"

#include <TGUI/Widgets/BitmapButton.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Picture.hpp>
#include <TGUI/Widgets/ScrollablePanel.hpp>
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "TGUI/Cursor.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/String.hpp"
#include "TGUI/Texture.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/ComboBox.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "TGUI/Widgets/GrowVerticalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/TextArea.hpp"
#include "bindTranslation.hpp"
#include "childWindows/addDelayDialogueWindow.hpp"
#include "childWindows/addDialogueOptionWindow.hpp"
#include "childWindows/colorSelectWindow.hpp"
#include "childWindows/editDialogueOptionWindow.hpp"
#include "dialogue.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "services/fileSystemService.hpp"
#include "services/translationService.hpp"
#include "variant.hpp"
#include "widgets/dialogueEditor.hpp"
#include "widgets/propertyFields/interPropField.hpp"

DialogueFileView::DialogueFileView() {
	dialogue = nullptr;

	TranslationService &ts = Editor::instance->getTranslations();
	noImageTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("no-image.png"));

	deleteTexture = tgui::Texture(Editor::instance->getFs().getResourcePath("delete.png"));

	auto toolsPanel = tgui::Panel::create({"100%", 32});
	toolsPanel->getRenderer()->setPadding({4, 4});
	newLineButton = tgui::Button::create();

	bindTranslation<tgui::Button>(newLineButton, "screen.project.dialogueview.add_new_line", &tgui::Button::setText);
	newLineButton->setSize("20%", "100%");
	toolsPanel->add(newLineButton);

	widgetContainer.push_back(toolsPanel);

	mainPanel = tgui::ScrollablePanel::create({"100%", "100% - 32"});
	mainPanel->setPosition(0, 32);
	mainPanel->getRenderer()->setPadding({16, 16});

	mainPanel->getVerticalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Automatic);
	mainPanel->getHorizontalScrollbar()->setPolicy(tgui::Scrollbar::Policy::Never);

	vertLayout = tgui::GrowVerticalLayout::create();
	vertLayout->getRenderer()->setSpaceBetweenWidgets(20.0f);
	mainPanel->add(vertLayout);
	widgetContainer.push_back(mainPanel);
}

constexpr int MAXIMUM_TEXT_SIZE = 32;
constexpr int MINIMUM_TEXT_SIZE = 16;

tgui::Panel::Ptr DialogueFileView::makeLinePanel(DialogueBin &data, DialogueLine line, size_t i) {
	TranslationService &ts = Editor::instance->getTranslations();
	auto panel = tgui::Panel::create({"100%", DIALOGUE_PANEL_HEIGHT});
	panel->getRenderer()->setPadding({16, 16});

	/// left group
	auto leftGroup = tgui::Group::create({"10%", "100%"});
	panel->add(leftGroup);

	tgui::Layout2d portraitLayout = {"100% - 40", "100% - 40"};

	auto portraitPic = tgui::Picture::create();
	portraitPic->setPosition(0, 40);
	portraitPic->setSize("height", "100% - 40");
	if (line.hasPortrait) {
		auto filePath = Editor::instance->getProject()->getResourcePath(EngineFileType::FILE_IMAGE, line.imageId);
		if (FileExists(filePath.c_str())) {
			tgui::Texture texture(filePath);
			portraitPic->getRenderer()->setTexture(texture);
		}
	} else {
		portraitPic->getRenderer()->setTexture(noImageTexture);
	}

	std::weak_ptr<tgui::Picture> portraitWeak = portraitPic;
	portraitPic->onClick([&data, this, i, portraitWeak] {
		if (data.lines.at(i).hasPortrait) {
			auto fileDialog = tgui::FileDialog::create();
			fileDialog->setFileTypeFilters({{"Images", {"*.png", "*.jpg"}}});
			fileDialog->onFileSelect([&data, this, i, portraitWeak](const tgui::String &path) {
				data.lines.at(i).imageId = GetFileName(path.toStdString().c_str());
				tgui::Texture texture(path);
				portraitWeak.lock()->getRenderer()->setTexture(texture);
				this->dirty = true;
			});

			Editor::instance->getGui().gui->add(fileDialog);
		}
	});
	leftGroup->add(portraitPic);

	/// center group
	auto centerGroup = tgui::Group::create({"80%", "100%"});
	centerGroup->setPosition({"10%", 0});
	panel->add(centerGroup);

	auto charNameEdit = tgui::EditBox::create();
	charNameEdit->setPosition({0, 0});
	charNameEdit->setSize({"10%", 36});
	charNameEdit->setText(line.characterName);
	charNameEdit->onTextChange(
		[&data, this, i](const tgui::String &text) {
			data.lines.at(i).characterName = text.toStdString();
			this->dirty = true;
		});
	centerGroup->add(charNameEdit);

	auto diagTextEdit = DialogueEditor::create();
	diagTextEdit->setMouseCursor(tgui::Cursor::Type::Text);
	diagTextEdit->setPosition({0, 40});
	diagTextEdit->setSize("50%", "100% - 40");
	diagTextEdit->setText(line.text);
	diagTextEdit->onTextChange([&data, this, i](const tgui::String &text) {
		data.lines.at(i).text = text.toStdString();
		this->dirty = true;
	});
	dialogueBoxes.push_back(diagTextEdit);

	centerGroup->add(diagTextEdit);

	auto selectColorButton = tgui::Button::create();
	selectColorButton->setPosition({"10% + 4", 0});
	selectColorButton->setSize("10%", 36);
	bindTranslation<tgui::Button>(selectColorButton, "screen.project.dialogueview.select_a_color",
								  &tgui::Button::setText);
	std::weak_ptr<DialogueEditor> weakEditor = diagTextEdit;
	selectColorButton->onPress.connect([weakEditor] {
		if (auto capture = weakEditor.lock()) {
			auto selectColorWindow = reinterpret_cast<ColorSelectWindow *>(
				Editor::instance->getGui().getChildWindowSubService()->getWindow("select_a_color"));

			selectColorWindow->open(capture);
		}
	});
	centerGroup->add(selectColorButton);

	auto addDelayButton = tgui::Button::create();
	addDelayButton->setPosition({"20% + 8", 0});
	addDelayButton->setSize("10%", 36);
	bindTranslation<tgui::Button>(addDelayButton, "screen.project.dialogueview.add_a_delay", &tgui::Button::setText);
	addDelayButton->onPress.connect([weakEditor] {
		if (auto capture = weakEditor.lock()) {
			auto delayWindow = reinterpret_cast<AddDelayDialogueWindow *>(
				Editor::instance->getGui().getChildWindowSubService()->getWindow("add_a_delay"));

			delayWindow->open(capture);
		}
	});
	centerGroup->add(addDelayButton);

	auto selectFontComboBox = tgui::ComboBox::create();
	selectFontComboBox->setPosition({"30% + 12", 0});
	selectFontComboBox->setSize("10%", 36);
	bindTranslation<tgui::ComboBox>(selectFontComboBox, "screen.project.dialogueview.select_a_font",
									&tgui::ComboBox::setDefaultText);

	Project *project = Editor::instance->getProject();
	auto fontPaths = project->getPaths(EngineFileType::FILE_FONT);

	std::for_each(fontPaths.begin(), fontPaths.end(), [&selectFontComboBox](const std::string &fontPath) {
		auto filename = GetFileNameWithoutExt(fontPath.c_str());
		selectFontComboBox->addItem(filename);
	});

	std::weak_ptr<tgui::ComboBox> weakFontBox = selectFontComboBox;
	selectFontComboBox->onItemSelect.connect([weakEditor, this, weakFontBox](const tgui::String &selectedIndex) {
		if (weakEditor.expired() || weakFontBox.expired()) {
			return;
		}

		auto editor = weakEditor.lock();
		auto box = weakFontBox.lock();

		editor->addXmlTagWithProperties("font", {{"font", selectedIndex.toStdString()}});
		this->dirty = true;
		box->deselectItem();
	});
	centerGroup->add(selectFontComboBox);

	auto textSizeComboBox = tgui::ComboBox::create();
	textSizeComboBox->setPosition({"40% + 16", 0});
	textSizeComboBox->setSize("10%", 36);

	// For convenience, this code calculates the wanted text sizes automatically.
	int textAddition = MAXIMUM_TEXT_SIZE / (MAXIMUM_TEXT_SIZE - MINIMUM_TEXT_SIZE);
	for (int i = MINIMUM_TEXT_SIZE; i <= MAXIMUM_TEXT_SIZE; i += textAddition) {
		textSizeComboBox->addItem(std::to_string(i));
	}

	bindTranslation<tgui::ComboBox>(textSizeComboBox, "screen.project.dialogueview.select_a_text_size",
									&tgui::ComboBox::setDefaultText);

	std::weak_ptr<tgui::ComboBox> weakTextSizeBox = textSizeComboBox;
	textSizeComboBox->onItemSelect.connect([weakEditor, this, weakTextSizeBox](const tgui::String &selectedIndex) {
		if (weakEditor.expired() || weakTextSizeBox.expired()) {
			return;
		}

		auto editor = weakEditor.lock();
		auto box = weakTextSizeBox.lock();

		editor->addXmlTagWithProperties("textSize", {{"size", selectedIndex.toStdString()}});
		box->deselectItem();
		this->dirty = true;
	});
	centerGroup->add(textSizeComboBox);

	auto addOptionButton = tgui::Button::create();
	bindTranslation<tgui::Button>(addOptionButton, "screen.project.dialogueview.add_option", &tgui::Button::setText);
	addOptionButton->setPosition("80% + 20", 0);
	addOptionButton->setSize(220, 36);
	addOptionButton->onClick([this, i] {
		auto *popupPtr = Editor::instance->getGui().getChildWindowSubService()->getWindow("add_dialogue_option");
		auto *addDialogueOptionWindow = static_cast<AddDialogueOptionWindow *>(popupPtr);

		addDialogueOptionWindow->dialogue = this->dialogue;
		addDialogueOptionWindow->lineIndex = i;
		addDialogueOptionWindow->fileView = this;

		addDialogueOptionWindow->open();
	});
	centerGroup->add(addOptionButton);

	auto optionsPanel = tgui::ScrollablePanel::create();
	optionsPanel->setPosition({"50% + 4", 32 + 8});
	optionsPanel->setSize("50% - 4", "100% - 40");

	auto optionsLayout = tgui::GrowVerticalLayout::create();
	optionsPanel->add(optionsLayout);

	optionPanels.push_back(optionsLayout);

	initOptionPanel(i);

	centerGroup->add(optionsPanel);

	auto hasImageCheck = tgui::CheckBox::create();
	bindTranslation<tgui::CheckBox>(hasImageCheck, "screen.project.dialogueview.has_a_portrait",
									&tgui::CheckBox::setText);
	hasImageCheck->setPosition({0, 0});
	hasImageCheck->setSize(32, 32);
	hasImageCheck->setChecked(line.hasPortrait);

	std::weak_ptr<tgui::TextArea> weakDiagText = diagTextEdit;
	hasImageCheck->onCheck([this, &data, i, portraitPic, weakDiagText](bool value) {
		data.lines.at(i).hasPortrait = value;
		if (!value) {
			data.lines.at(i).imageId = "";
			if (!weakDiagText.expired()) {
				weakDiagText.lock()->setSize({"100%", "100% - 40"});
				weakDiagText.lock()->setPosition(0, 32 + 8);
			}

			portraitPic->getRenderer()->setTexture(noImageTexture);
		} else {
			if (!weakDiagText.expired()) {
				weakDiagText.lock()->setSize({"100% - 210 - 28", "100% - 40"});
				weakDiagText.lock()->setPosition(210, 32 + 8);
			}
		}
	});
	leftGroup->add(hasImageCheck);

	auto deleteButton = tgui::BitmapButton::create();
	deleteButton->setSize({32, 32});
	deleteButton->setPosition("100% - 32", 0);
	deleteButton->setImage(deleteTexture);
	deleteButton->onClick([this, &data, i] {
		mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size()))});

		data.lines.erase(data.lines.begin() + i);
		vertLayout->remove(linePanels.at(i));
		linePanels.erase(linePanels.begin() + i);
		this->dirty = true;
	});
	panel->add(deleteButton);

	return panel;
}

void DialogueFileView::initOptionPanel(int lineIndex) {
	auto &line = dialogue->getData().lines[lineIndex];
	auto &panel = optionPanels.at(lineIndex);
	panel->removeAllWidgets();

	if (line.hasOptions) {
		int i = 0;
		for (auto &option : line.options) {
			auto optionItem = InterPropField::create();
			optionItem->setSize({"100%", 32});
			optionItem->label->setText(option.title);
			optionItem->value->setText(option.nextDialogue);

			optionItem->value->onClick([this, lineIndex, i] {
				auto *popupPtr =
					Editor::instance->getGui().getChildWindowSubService()->getWindow("edit_dialogue_option");
				auto *editDialogueOptionWindow = static_cast<EditDialogueOptionWindow *>(popupPtr);

				editDialogueOptionWindow->setup(dialogue, lineIndex, i);
				editDialogueOptionWindow->fileView = this;
				editDialogueOptionWindow->open();
			});

			optionItem->remove->onClick([this, i, lineIndex, &line] {
				line.options.erase(line.options.begin() + i);
				initOptionPanel(lineIndex);
			});

			panel->add(optionItem);
			i++;
		}
	}
}

void DialogueFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	this->variant = variant;

	if (variant != nullptr) {
		const auto ptr = dynamic_cast<Variant<Dialogue> *>(variant);
		const auto dialogue = ptr->get();

		this->dialogue = dialogue;

		auto &data = dialogue->getData();

		newLineButton->onClick([this, &dialogue, &data] {
			DialogueLine newLine;
			newLine.characterName = "Character";
			newLine.hasPortrait = false;
			newLine.imageId = "";
			newLine.text = "Enter text here!";

			data.lines.push_back(newLine);

			auto panel = makeLinePanel(data, newLine, data.lines.size() - 1);

			vertLayout->add(panel);
			linePanels.push_back(panel);

			mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size() + 1))});
		});

		mainPanel->setContentSize({0, static_cast<float>((DIALOGUE_PANEL_HEIGHT + 8) * (data.lines.size() + 1))});

		int i = 0;
		for (auto line : dialogue->getData().lines) {
			auto panel = makeLinePanel(data, line, i);

			i++;

			vertLayout->add(panel);
			linePanels.push_back(panel);
		}

		addWidgets(layout);
		this->dirty = false;
	}
}
