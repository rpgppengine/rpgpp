#include "fileViews/imageFileView.hpp"

#include <TGUI/Widgets/Panel.hpp>

#include "TGUI/Widgets/Label.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "saveables/imageWrapper.hpp"
#include "views/imageView.hpp"
#include "views/worldView.hpp"

ImageFileView::ImageFileView() {
	imageView = ImageView::create();
	imageView->setPosition(0, 36);
	imageView->setSize({"100%", "100% - 36"});

	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(imageView));

	auto topPanel = tgui::Panel::create();
	topPanel->setSize("100%", 36);
	topPanel->getRenderer()->setPadding({4, 4});

	infoLabel = tgui::Label::create();
	infoLabel->setSize({"100%", "100%"});
	infoLabel->setVerticalAlignment(tgui::VerticalAlignment::Center);
	topPanel->add(infoLabel);

	widgetContainer.push_back(topPanel);
	widgetContainer.push_back(imageView);
}

void ImageFileView::init(tgui::Group::Ptr layout, VariantWrapper *variant) {
	if (variant == nullptr) {
		return;
	}

	auto ptr = dynamic_cast<Variant<ImageWrapper> *>(variant);
	auto image = ptr->get();

	imageView->setImage(image);
	infoLabel->setText(
		TextFormat("%s | %ix%i", GetFileName(image->source.c_str()), image->image.width, image->image.height));

	addWidgets(layout);
}
