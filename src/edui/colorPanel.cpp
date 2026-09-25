#include "edui/colorPanel.hpp"

#include <cstdio>
#include <memory>

#include "edui/button.hpp"
#include "edui/colorRect.hpp"
#include "edui/colorWheel.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/contextMenu.hpp"
#include "edui/gui.hpp"
#include "edui/intValue.hpp"
#include "edui/label.hpp"
#include "edui/messageBox.hpp"
#include "edui/slider.hpp"
#include "raylib.h"

using namespace edui;

ColorPanel::ColorPanel() : ConfirmDialog() {
	// deleteOnOutsideClick = true;
	render = std::make_unique<ColorPanelRender>();
	render->padding = 4;

	setSize({0, 320}, {0, 250});

	setText("");
	setTitle("Select Color..");

	colorWheel = std::make_shared<edui::ColorWheel>();
	colorWheel->setSize({0, 100}, {0, 100});
	colorWheel->render->border = 0;
	colorWheel->onColorChanged.connect([this](Color newColor) {
		this->color = newColor;

		colorRect->color = color;
		updateLabels();
		updateSliders();
		updateValues();
	});
	add(colorWheel);

	initLabelWidgets();
	initValueWidgets();

	colorRect = std::make_shared<edui::ColorRect>();
	colorRect->setPosition({0, 210}, {0, 104});
	colorRect->setSize({0, 54}, {0, 54});
	add(colorRect);

	edui::Button &button = footerLayout->widgets[1]->as<edui::Button>();
	button.onClicked.disconnectAll();
	button.onClicked.connect([this] {
		onColorConfirm.invoke(color);
		markDelete();
		footerLayout->markDelete();
	});

	// init color
	color = WHITE;
	setColor(color);
}

void ColorPanel::setColor(Color color) {
	this->color = color;
	colorWheel->setColor(color);
	updateLabels();
	updateSliders();
	updateValues();
	colorRect->color = color;
}

void ColorPanel::initLabelWidgets() {
	rgbLabel = std::make_shared<edui::Label>();
	rgbLabel->render->border = 0;
	rgbLabel->setPosition({0, 0}, {0, 104});
	rgbLabel->setSize({0, 100}, {0, 20});
	rgbLabel->setText("rgb()");
	rgbLabel->onRightClicked.connect([this] { openContextMenu(); });
	add(rgbLabel);

	hexLabel = std::make_shared<edui::Label>();
	hexLabel->render->border = 0;
	hexLabel->setPosition({0, 0}, {0, 128});
	hexLabel->setSize({0, 100}, {0, 20});
	hexLabel->setText("#");
	hexLabel->onRightClicked.connect([this] { openContextMenu(); });
	add(hexLabel);
}

void ColorPanel::initValueWidgets() {
	// red
	redSlider = std::make_shared<edui::Slider>();
	redSlider->setMaxValue(255);
	redSlider->setPosition({0, 104}, {0, 2});
	redSlider->setSize({1, -180}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	redSlider->onValueChanged([this](int newValue) {
		color.r = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateValues();
	});
	add(redSlider);

	redValue = std::make_shared<edui::IntValue>();
	redValue->intval->max = 255;
	redValue->setPosition({1, -75}, {0, 2});
	redValue->setSize({0, 75}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	redValue->onValueChangedT([this](int old, int newValue) {
		color.r = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateSliders();
	});
	add(redValue);

	// green
	greenSlider = std::make_shared<edui::Slider>();
	greenSlider->setMaxValue(255);
	greenSlider->setPosition({0, 104}, {0, static_cast<int>(2 + (EDUI_SECONDARY_HEIGHT + 4))});
	greenSlider->setSize({1, -180}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	greenSlider->onValueChanged([this](int newValue) {
		color.g = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateValues();
	});
	add(greenSlider);

	greenValue = std::make_shared<edui::IntValue>();
	greenValue->intval->max = 255;
	greenValue->setPosition({1, -75}, {0, static_cast<int>(2 + EDUI_SECONDARY_HEIGHT + 4)});
	greenValue->setSize({0, 75}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	greenValue->onValueChangedT([this](int old, int newValue) {
		color.g = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateSliders();
	});
	add(greenValue);

	// blue
	blueSlider = std::make_shared<edui::Slider>();
	blueSlider->setMaxValue(255);
	blueSlider->setPosition({0, 104}, {0, static_cast<int>(2 + (EDUI_SECONDARY_HEIGHT * 2) + 8)});
	blueSlider->setSize({1, -180}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	blueSlider->onValueChanged([this](int newValue) {
		color.b = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateValues();
	});
	add(blueSlider);

	blueValue = std::make_shared<edui::IntValue>();
	blueValue->intval->max = 255;
	blueValue->setPosition({1, -75}, {0, static_cast<int>(2 + EDUI_SECONDARY_HEIGHT * 2) + 8});
	blueValue->setSize({0, 75}, {0, static_cast<int>(EDUI_SECONDARY_HEIGHT)});
	blueValue->onValueChangedT([this](int old, int newValue) {
		color.b = newValue;
		colorRect->color = color;
		colorWheel->setColor(color);
		updateLabels();
		updateSliders();
	});
	add(blueValue);
}

void ColorPanel::openContextMenu() {
	if (contextMenu != nullptr) {
		contextMenu->markDelete();
	}
	contextMenu = std::make_shared<edui::ContextMenu>();
	contextMenu->moveToMouse();
	contextMenu->addItem("Copy RGB");
	contextMenu->addItem("Copy Hex");
	contextMenu->onItemClicked([this](const std::string &item) {
		if (item == "Copy RGB") {
			std::string colorString = TextFormat("%u, %u, %u", color.r, color.g, color.b);
			SetClipboardText(colorString.c_str());
		}
		if (item == "Copy Hex") {
			std::string colorString = TextFormat("#%x", ColorToInt(color));
			SetClipboardText(colorString.c_str());
		}
	});
	edui::Gui::instance->add(contextMenu, layerId + 1);
	// will not add for now
}

void ColorPanel::updateLabels() {
	rgbLabel->setText(TextFormat("rgb(%u, %u, %u)", color.r, color.g, color.b));
	rgbLabel->setWidthFit();

	hexLabel->setText(TextFormat("#%x", ColorToInt(color)));
	hexLabel->setWidthFit();
}

void ColorPanel::updateSliders() {
	redSlider->setValue(color.r);
	greenSlider->setValue(color.g);
	blueSlider->setValue(color.b);
}

void ColorPanel::updateValues() {
	redValue->setValue(color.r);
	greenValue->setValue(color.g);
	blueValue->setValue(color.b);
}
