#ifndef _EDUI_FILEVALUE_H
#define _EDUI_FILEVALUE_H

#include <memory>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/fileChooser.hpp"
#include "edui/valuewidget.hpp"
namespace edui {
struct FileOption {
	std::string fullPath;
	bool filled = false;

	operator bool() { return filled; }
};

struct FileValueRender : public ContainerRender {};

struct FileValue : public Container, public ValueWidgetT<FileOption> {
	const int FILE_CHOOSER_WIDTH = 600;
	const int FILE_CHOOSER_HEIGHT = 400;

	void setValue(const FileOption &newValue) override;
	FileOption getValue() override;

	FileValue();
	void translate() override;

	void setEmpty();
	void setFromPath(const std::string &fullPath);

	void addCategory(const std::string &title, std::vector<std::string> extensions, int iconId);

	void addImagesCategory();
	void addTextCategory();
	void addAudioCategory();
	void addFontCategory();

private:
	std::vector<FileCategory> categories = {};

	FileOption file;

	std::shared_ptr<Button> button = nullptr;
	void onButtonClicked();
};
}  // namespace edui

#endif
