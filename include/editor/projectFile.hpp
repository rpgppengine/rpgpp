#ifndef _RPGPP_PROJECTFILE_H
#define _RPGPP_PROJECTFILE_H

#include <memory>

#include "TGUI/Widgets/Group.hpp"
#include "fileViews/fileView.hpp"
#include "services/fileSystemService.hpp"
#include "variant.hpp"

class ProjectFile {
private:
	std::unique_ptr<FileView> view;
	std::unique_ptr<VariantWrapper> variant;
	EngineFileType fileType;
	std::string filePath;
	bool isSaveable = true;

public:
	bool isEmpty = false;
	ProjectFile();
	ProjectFile(std::unique_ptr<FileView> view, std::unique_ptr<VariantWrapper> variant, EngineFileType fileType,
				bool isSaveable = true);
	void setFilePath(const std::string &filePath);
	std::string &getFilePath();
	void initUi(tgui::Group::Ptr group);
	void addWidgets(tgui::Group::Ptr layout);
	void saveFile(const std::string &path);
	FileView &getView();
	FileView* getViewPtr();
	EngineFileType getFileType();
};

#endif
