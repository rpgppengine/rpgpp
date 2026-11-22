#ifndef _EDUI_FILEINPUT_H
#define _EDUI_FILEINPUT_H

#include <filesystem>
#include <memory>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/fileChooser.hpp"
namespace edui {
struct FileOption {
	std::string fullPath;
	bool filled = false;

	FileOption() {}
	FileOption(const std::string &path) {
		filled = !fullPath.empty();
		fullPath = path;
	}
	FileOption(const std::string &path, bool filled) {
		fullPath = path;
		this->filled = filled;
	}

	operator bool() { return filled; }
};

struct FileInputRender : public ContainerRender {};

struct FileInput : public Container {
	SignalT2<FileOption, FileOption> onValueChanged;
	Signal fileValueEmptied;

	void setValue(const FileOption &newValue);
	FileOption getValue();

	FileInput();
	void translate() override;

	void setEmpty();
	void setFromPath(const std::string &fullPath, bool signal = true);

	void addCategory(const std::string &title, std::vector<std::string> extensions, int iconId);

	void addImagesCategory();
	void addTextCategory();
	void addAudioCategory();
	void addFontCategory();

	void setDirSelection(bool val = true);
	void lockBasePath(const std::filesystem::path &basePath);

private:
	bool dirSelection = false;
	std::vector<FileCategory> categories = {};
	std::filesystem::path basePath = "";
	bool useBasePath = false;

	FileOption file;

	std::shared_ptr<Button> button = nullptr;
	void onButtonClicked();
};
}  // namespace edui

#endif
