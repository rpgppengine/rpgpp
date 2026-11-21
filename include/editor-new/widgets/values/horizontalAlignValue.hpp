#ifndef _EDITOR_WIDGETS_VALUES_HORIZONTALALIGNVALUE_H
#define _EDITOR_WIDGETS_VALUES_HORIZONTALALIGNVALUE_H

#include <memory>

#include "basicTypes.hpp"
#include "edui/dropdown.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct HorizontalAlignValue : public ValueWidgetT<HorizontalAlignment> {
	HorizontalAlignValue();

	void setValue(const HorizontalAlignment &value) override;
	HorizontalAlignment getValue() override;

private:
	std::shared_ptr<edui::Dropdown> dropdown;
};
}  // namespace editor

#endif
