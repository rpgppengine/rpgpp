#ifndef _EDITOR_WIDGETS_VALUES_IMAGEREFVALUE_H
#define _EDITOR_WIDGETS_VALUES_IMAGEREFVALUE_H

#include <memory>

#include "basicTypes.hpp"
#include "edui/fileInput.hpp"
#include "widgets/values/valuewidget.hpp"
namespace editor {
struct ImageRefValue : public ValueWidgetT<ImageRef> {
	ImageRefValue();

	void setValue(const ImageRef &value) override;
	ImageRef getValue() override;

private:
	std::shared_ptr<edui::FileInput> input;
};
}  // namespace editor

#endif
