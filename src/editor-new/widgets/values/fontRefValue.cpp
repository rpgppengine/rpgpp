#include "widgets/values/fontRefValue.hpp"

#include <memory>

#include "basicTypes.hpp"
#include "editor.hpp"
#include "edui/fileInput.hpp"

using namespace editor;

FontRefValue::FontRefValue() {
	input = std::make_shared<edui::FileInput>();
	input->setSize({1, 0}, {1, 0});

	input->lockBasePath(Editor::getInstance().getResources().getProject().getBasePath());
	input->addFontCategory();

	input->onValueChanged.connect([this](edui::FileOption a, edui::FileOption b) {
		FontRef old = {a.fullPath};
		FontRef newval = {b.fullPath};

		onValueChanged.invoke(old, newval);
	});

	add(input);
}

FontRef FontRefValue::getValue() { return {input->getValue().fullPath}; }

void FontRefValue::setValue(const FontRef &value) { input->setValue({value.path}); }
