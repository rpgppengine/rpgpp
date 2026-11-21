#ifndef _EDITOR_WIDGETS_VALUES_STRINGVALUE_H
#define _EDITOR_WIDGETS_VALUES_STRINGVALUE_H

#include <memory>
#include <string>

#include "edui/textBox.hpp"
#include "widgets/values/valuewidget.hpp"

namespace editor {
struct StringValue : ValueWidgetT<std::string> {
	StringValue();

	void setValue(const std::string &value) override;
	std::string getValue() override;

private:
	std::shared_ptr<edui::TextBox> textBox;
};
}  // namespace editor

#endif
