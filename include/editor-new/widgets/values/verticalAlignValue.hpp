#ifndef _EDITOR_WIDGETS_VALUES_VERTICALALIGNVALUE_H
#define _EDITOR_WIDGETS_VALUES_VERTICALALIGNVALUE_H

#include <memory>

#include "basicTypes.hpp"
#include "edui/dropdown.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct VerticalAlignValue : public ValueWidgetT<VerticalAlignment> {
	VerticalAlignValue();

	void setValue(const VerticalAlignment &value) override;
	VerticalAlignment getValue() override;

private:
	std::shared_ptr<edui::Dropdown> dropdown;
};
}  // namespace editor

#endif
