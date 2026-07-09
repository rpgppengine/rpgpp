#include "projectFile.hpp"

#include <memory>
#include <string>
#include <utility>

#include "TGUI/Widgets/Group.hpp"
#include "fileViews/emptyView.hpp"
#include "fileViews/fileView.hpp"
#include "raylib.h"
#include "services/fileSystemService.hpp"
#include "tileset.hpp"
#include "variant.hpp"

ProjectFile::ProjectFile() {
	view = std::make_unique<EmptyFileView>();
}

ProjectFile::ProjectFile(std::unique_ptr<FileView> view, std::unique_ptr<VariantWrapper> variant,
						 EngineFileType fileType, bool isSaveable) {
	this->view = std::move(view);
	this->variant = std::move(variant);
	this->fileType = fileType;
	this->isSaveable = isSaveable;
}

void ProjectFile::setFilePath(const std::string &filePath) { this->filePath = filePath; }

std::string &ProjectFile::getFilePath() { return filePath; }

void ProjectFile::initUi(tgui::Group::Ptr group) {
	if (variant.get() == nullptr) {
		view->init(group, nullptr);
	} else {
		view->init(group, variant.get());
	}
}

void ProjectFile::addWidgets(tgui::Group::Ptr layout) { view->addWidgets(layout); }

void ProjectFile::saveFile(const std::string &path) {
	if (!isSaveable) return;

	auto saveable = variant->toSaveable();
	json j = saveable->dumpJson();
	SaveFileText(path.c_str(), j.dump().c_str());
}

FileView &ProjectFile::getView() { return *view; }

FileView* ProjectFile::getViewPtr() { return view.get(); }
