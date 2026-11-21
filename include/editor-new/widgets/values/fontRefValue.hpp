#ifndef _EDITOR_VALUES_FONTREFVALUE_H
#define _EDITOR_VALUES_FONTREFVALUE_H

#include <memory>

#include "basicTypes.hpp"
#include "edui/fileInput.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct FontRefValue : public ValueWidgetT<FontRef> {
	FontRefValue();

	void setValue(const FontRef &value) override;
	FontRef getValue() override;

private:
	std::shared_ptr<edui::FileInput> input;
};
}  // namespace editor

#endif
