#ifndef _EDITOR_WIDGETS_FILEPANELS_FILEPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_FILEPANEL_H

#include <filesystem>
#include <memory>

#include "edui/container.hpp"
#include "resource.hpp"
namespace editor {
class FilePanel : public edui::Container {
public:
	FilePanel();
	void setSourcePath(const std::filesystem::path &path);
	std::filesystem::path getSourcePath();
	void setResource(std::unique_ptr<Resource> ptr);
	virtual void init() {};
	virtual void onOpen() {};
	virtual void onClose() {};

protected:
	std::filesystem::path sourcePath;
	std::unique_ptr<Resource> resource;
};
}  // namespace editor

#endif
