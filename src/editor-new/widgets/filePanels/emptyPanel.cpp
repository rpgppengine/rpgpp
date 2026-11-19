#include "widgets/filePanels/emptyPanel.hpp"

#include <memory>

#include "edui/label.hpp"
#include "widgets/filePanels/filePanel.hpp"

using namespace editor;

EmptyPanel::EmptyPanel() : FilePanel() {
	auto label = std::make_shared<edui::Label>();
	label->setText("EmptyPanel");
	label->setSize({1, 0}, {1, 0});
	label->render->as<edui::LabelRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;
	label->render->as<edui::LabelRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	add(label);
}
