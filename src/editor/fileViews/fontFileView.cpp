#include "fileViews/fontFileView.hpp"

#include <cmath>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/EditBox.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "TGUI/Widgets/SpinControl.hpp"
#include "editor.hpp"
#include "saveables/fontWrapper.hpp"
#include "variant.hpp"
#include "views/fontView.hpp"
#include "views/worldView.hpp"

FontFileView::FontFileView() {
	fontView = FontView::create();
	fontView->setPosition(0, 36);
	fontView->setSize({"100%", "100% - 36"});

	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(fontView));

	auto topPanel = tgui::Panel::create();
	topPanel->setSize("100%", 36);
	topPanel->getRenderer()->setPadding({4, 4});

	auto fontSizeField = tgui::SpinControl::create();
	fontSizeField->setSize({120.0f, "100%"});
	fontSizeField->setMinimum(8.0f);
	fontSizeField->setMaximum(160.0);
	fontSizeField->setValue(16.0f);
	fontSizeField->onValueChange([this](float value) { fontView->setBaseFontSize(static_cast<int>(floor(value))); });
	topPanel->add(fontSizeField);

	auto fontTextField = tgui::EditBox::create();
	fontTextField->setSize("100% - 140", "100%");
	fontTextField->setPosition(124.0f, 0);
	fontTextField->setText(fontView->getText());
	fontTextField->onTextChange([this](const tgui::String &text) { fontView->setText(text.toStdString()); });
	topPanel->add(fontTextField);

	widgetContainer.push_back(fontView);
	widgetContainer.push_back(topPanel);
}

void FontFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	if (variant == nullptr) {
		return;
	}

	auto ptr = dynamic_cast<Variant<FontWrapper> *>(variant);
	auto font = ptr->get();

	fontView->setFont(font);

	addWidgets(layout);
}
