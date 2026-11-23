#ifndef _EDITOR_WIDGETS_FILEPANELS_FILEPANEL_H
#define _EDITOR_WIDGETS_FILEPANELS_FILEPANEL_H

#include <filesystem>
#include <memory>

#include "edui/container.hpp"
#include "edui/signal.hpp"
#include "resource.hpp"
#include "utils/command.hpp"
#include "utils/commandHistory.hpp"
namespace editor {
class FilePanel : public edui::Container {
public:
	edui::SignalT<bool> dirty;

	FilePanel();
	void setSourcePath(const std::filesystem::path &path);
	std::filesystem::path getSourcePath();
	void setResource(std::unique_ptr<Resource> ptr);
	virtual void init() {};
	virtual void refresh() {};
	CommandHistory &getHistory();
	void undo();
	void redo();
	void saveFile();

protected:
	std::filesystem::path sourcePath;
	std::unique_ptr<Resource> resource;
	CommandHistory history;

	void exec(std::unique_ptr<Command> cmd);
};
}  // namespace editor

#endif
