#include "widgets/values/imageRefValue.hpp"

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/fileInput.hpp"

using namespace editor;

ImageRefValue::ImageRefValue() {
	input = std::make_shared<edui::FileInput>();
	input->setSize({1, 0}, {1, 0});

	input->lockBasePath(Editor::getInstance().getResources().getProject().getBasePath());
	input->addImagesCategory();

	input->onValueChanged.connect([this](edui::FileOption a, edui::FileOption b) {
		ImageRef old = {a.fullPath};
		ImageRef newval = {b.fullPath};

		onValueChanged.invoke(old, newval);
	});

	add(input);
}

void ImageRefValue::setValue(const ImageRef &value) { input->setValue({value.path}); }

ImageRef ImageRefValue::getValue() { return {input->getValue().fullPath}; }
