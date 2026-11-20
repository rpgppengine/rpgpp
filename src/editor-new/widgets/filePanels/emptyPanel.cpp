#include "widgets/filePanels/emptyPanel.hpp"

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/fileValue.hpp"
#include "edui/gui.hpp"
#include "edui/label.hpp"
#include "widgets/filePanels/filePanel.hpp"
#include "widgets/propertyList.hpp"

using namespace editor;

EmptyPanel::EmptyPanel() : FilePanel() {
	auto label = std::make_shared<edui::Label>();
	label->setText("EmptyPanel");
	label->setSize({1, 0}, {1, 0});
	label->render->as<edui::LabelRender>().horiAlign = edui::HorizontalAlignment::TEXT_MIDDLE;
	label->render->as<edui::LabelRender>().vertAlign = edui::VerticalAlignment::TEXT_CENTER;
	add(label);

	auto fileValue = std::make_shared<edui::FileValue>();
	fileValue->setSize({0, 100}, {0, edui::EDUI_SECONDARY_HEIGHT});
	fileValue->addImagesCategory();
	fileValue->lockBasePath(Editor::getInstance().getResources().getProject().getBasePath());
	add(fileValue);

	auto propertyList = std::make_shared<PropertyList>();
	propertyList->setSize({0, 220}, {0.5, 0});
	add(propertyList);

	ElementProperty prop = 1;
	propertyList->addProperty(prop);
}
