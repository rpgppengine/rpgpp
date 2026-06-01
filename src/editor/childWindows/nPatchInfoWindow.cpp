#include "childWindows/nPatchInfoWindow.hpp"

#include "TGUI/Widgets/GrowHorizontalLayout.hpp"
#include "TGUI/Widgets/Panel.hpp"
#include "childWindows/popupWindow.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "views/nPatchView.hpp"
#include "views/worldView.hpp"
#include "widgets/propertyFields/intField.hpp"

NPatchInfoWindow::NPatchInfoWindow() : PopupWindow("NPatchInfoWindow") {
	currentWindow->setSize(520, 370);

	view = NPatchView::create();
	view->setSize({"100%", "100% - 32"});
	view->setPosition(0, 32);

	Editor::instance->getGui().addUpdate(WorldView::asUpdatable(view));
	currentWindow->add(view);

	topPanel = tgui::Panel::create();
	topPanel->setSize({"100%", 36});
	auto layout = tgui::GrowHorizontalLayout::create();
	topPanel->add(layout);
	currentWindow->add(topPanel);

	top = IntField::create();
	top->setSize({FIELD_WIDTH, "100%"});
	top->label->setText("Top");
	top->value->onValueChange([this](int value) { view->info->top = value; });
	layout->add(top);

	left = IntField::create();
	left->setSize({FIELD_WIDTH, "100%"});
	left->label->setText("Left");
	left->value->onValueChange([this](int value) { view->info->left = value; });
	layout->add(left);

	bottom = IntField::create();
	bottom->setSize({FIELD_WIDTH, "100%"});
	bottom->label->setText("Bottom");
	bottom->value->onValueChange([this](int value) { view->info->bottom = value; });
	layout->add(bottom);

	right = IntField::create();
	right->setSize({FIELD_WIDTH, "100%"});
	right->label->setText("Right");
	right->value->onValueChange([this](int value) { view->info->right = value; });
	layout->add(right);

	view->onInfoChanged([this](NPatchInfo *info) { init(info, view->texture); });
}

void NPatchInfoWindow::init(NPatchInfo *info, Texture texture) {
	top->value->setMinimum(0);
	top->value->setMaximum(texture.height);
	top->value->setValue(info->top);

	left->value->setMinimum(0);
	left->value->setMaximum(texture.width);
	left->value->setValue(info->left);

	bottom->value->setMinimum(0);
	bottom->value->setMaximum(texture.height);
	bottom->value->setValue(info->bottom);

	right->value->setMinimum(0);
	right->value->setMaximum(texture.width);
	right->value->setValue(info->right);
}
