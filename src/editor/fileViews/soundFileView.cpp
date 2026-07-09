#include "fileViews/soundFileView.hpp"

#include <TGUI/Widgets/Panel.hpp>
#include <memory>
#include <string>

#include "editor.hpp"
#include "raylib.h"
#include "saveables/soundWrapper.hpp"
#include "timeFormat.hpp"
#include "updatable.hpp"
#include "widgets/soundPlayer.hpp"

SoundFileView::SoundFileView() {
	auto topPanel = tgui::Panel::create();
	topPanel->setSize("100%", 36);
	topPanel->getRenderer()->setPadding({4, 4});

	infoLabel = tgui::Label::create();
	infoLabel->setSize({"100%", "100%"});
	infoLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	topPanel->add(infoLabel);

	soundPlayer = SoundPlayer::create();
	soundPlayer->setPosition(20, 36 + 20);
	soundPlayer->setSize({"100% - 40", 48});

	Editor::instance->getGui().addUpdate(dynamic_pointer_cast<IUpdatable>(soundPlayer));

	widgetContainer.push_back(topPanel);
	widgetContainer.push_back(soundPlayer);
}

void SoundFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	if (variant == nullptr) {
		return;
	}

	auto ptr = dynamic_cast<Variant<SoundWrapper> *>(variant);
	auto sound = ptr->get();

	infoLabel->setText(TextFormat("%s | Length: %s", GetFileName(sound->source.c_str()),
								  formatTime(GetMusicTimeLength(sound->sound)).c_str()));
	soundPlayer->setSound(sound);

	addWidgets(layout);
}
