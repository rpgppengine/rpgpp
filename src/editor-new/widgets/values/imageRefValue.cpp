#include "widgets/values/imageRefValue.hpp"

#include <filesystem>
#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/fileInput.hpp"

using namespace editor;

ImageRefValue::ImageRefValue() {
	auto projectBase = Editor::getInstance().getResources().getProject().getBasePath();

	input = std::make_shared<edui::FileInput>();
	input->setSize({1, 0}, {1, 0});

	input->lockBasePath(projectBase);
	input->addImagesCategory();

	input->onValueChanged.connect([this, projectBase](edui::FileOption a, edui::FileOption b) {
		std::filesystem::path oldPath = a.fullPath;
		std::filesystem::path newPath = b.fullPath;
		if (oldPath.is_absolute()) {
			oldPath = std::filesystem::relative(a.fullPath);
		}
		if (newPath.is_absolute()) {
			newPath = std::filesystem::relative(b.fullPath);
		}

		ImageRef old = {oldPath};
		ImageRef newval = {newPath};

		onValueChanged.invoke(old, newval);
	});

	add(input);
}

void ImageRefValue::setValue(const ImageRef &value) { input->setValue({value.path}); }

ImageRef ImageRefValue::getValue() { return {input->getValue().fullPath}; }
