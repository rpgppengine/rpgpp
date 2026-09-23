#ifndef _EDUI_COLORPANEL_H
#define _EDUI_COLORPANEL_H

#include <memory>

#include "edui/childWindow.hpp"
#include "edui/colorRect.hpp"
#include "edui/colorWheel.hpp"
#include "edui/confirmDialog.hpp"
#include "edui/contextMenu.hpp"
#include "edui/intValue.hpp"
#include "edui/label.hpp"
#include "edui/messageBox.hpp"
#include "edui/signal.hpp"
#include "edui/slider.hpp"

namespace edui {
struct ColorPanelRender : public ConfirmDialogRender {};

struct ColorPanel : public ConfirmDialog {
	SignalT<Color> onColorConfirm;

	std::shared_ptr<edui::ColorWheel> colorWheel = nullptr;
	std::shared_ptr<edui::Label> rgbLabel = nullptr;
	std::shared_ptr<edui::Label> hexLabel = nullptr;

	std::shared_ptr<edui::Slider> redSlider = nullptr;
	std::shared_ptr<edui::IntValue> redValue = nullptr;

	std::shared_ptr<edui::Slider> greenSlider = nullptr;
	std::shared_ptr<edui::IntValue> greenValue = nullptr;

	std::shared_ptr<edui::Slider> blueSlider = nullptr;
	std::shared_ptr<edui::IntValue> blueValue = nullptr;

	std::shared_ptr<edui::ColorRect> colorRect = nullptr;

	std::shared_ptr<edui::ContextMenu> contextMenu = nullptr;

	ColorPanel();

	void setColor(Color color);

private:
	Color color;

	void initLabelWidgets();
	void initValueWidgets();
	void openContextMenu();
	void updateLabels();
	void updateSliders();
	void updateValues();
};
}  // namespace edui

#endif
