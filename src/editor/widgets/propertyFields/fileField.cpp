#include "widgets/propertyFields/fileField.hpp"

#include <memory>
#include <string>

#include "TGUI/String.hpp"
#include "TGUI/Widgets/Button.hpp"
#include "TGUI/Widgets/FileDialog.hpp"
#include "editor.hpp"
#include "raylib.h"
#include "widgets/propertyFields/fieldConfig.hpp"

FileField::FileField(const char *typeName, bool initRenderer) : tgui::SubwidgetContainer(typeName, initRenderer) {
	label = tgui::Label::create("Label");
	label->setHorizontalAlignment(tgui::HorizontalAlignment::Left);
	label->setVerticalAlignment(tgui::VerticalAlignment::Center);

	value = tgui::Button::create();
	value->onPress([this] {
		auto fileDialog = tgui::FileDialog::create();
		fileDialog->setPath(Editor::instance->getProject()->getBasePath());
		fileDialog->setFileTypeFilters(pathFilters);
		fileDialog->onFileSelect([this](const tgui::String &path) {
			value->setText(std::string(GetFileName(path.toStdString().c_str())));
			chosenPath = path;
			if (callback != nullptr) {
				callback(path);
			}
		});

		Editor::instance->getGui().gui->add(fileDialog);
	});

	m_container->add(label);
	m_container->add(value);

	updateSize();
}

FileField::Ptr FileField::create() { return std::make_shared<FileField>(); }

FileField::Ptr FileField::create(const tgui::String &label, const tgui::String &value) {
	auto ptr = std::make_shared<FileField>();
	ptr->label->setText(label);
	ptr->value->setText(value);
	return ptr;
}

FileField::Ptr FileField::copy(FileField::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<FileField>(widget->clone());
	} else {
		return nullptr;
	}
}

tgui::Widget::Ptr FileField::clone() const { return std::make_shared<FileField>(*this); }

void FileField::setValue(const tgui::String &value) {
	this->value->setText(value);
	chosenPath = value;
}

void FileField::setSize(const tgui::Layout2d &size) {
	tgui::SubwidgetContainer::setSize(size);
	updateSize();
}

void FileField::updateSize() {
	label->setPosition({PADDING, 0});
	label->setSize({getSize().x * 0.5f - PADDING, getSize().y});
	value->setSize({getSize().x * 0.5f - PADDING, getSize().y});

	value->setPosition({getSize().x * 0.5, 0});
}

tgui::String &FileField::getChosenPath() { return chosenPath; }
