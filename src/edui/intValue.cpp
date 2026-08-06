#include "edui/intValue.hpp"

#include <memory>

#include "edui/button.hpp"
#include "edui/container.hpp"
#include "edui/intValueTextBox.hpp"
#include "edui/label.hpp"
#include "edui/widget.hpp"

using namespace edui;

IntValue::IntValue() : Container() {
	isScissor = false;
	render = std::make_unique<IntValueRender>();

	intval = std::make_shared<edui::IntValueTextBox>();
	intval->setPosition({0, 0}, {0, 0});
	intval->setSize({1.0f, -24}, {1, 0});
	intval->intValueChanged = [this](int a, int b) { onValueChangedT.invoke(a, b); };
	add(intval);

	std::weak_ptr<edui::IntValueTextBox> weakIntVal = intval;

	incButton = std::make_shared<edui::Button>();
	incButton->setText("");
	incButton->setPosition({1.0f, -24}, {0, 0});
	incButton->setSize({0, 24}, {0.5f, 0});
	incButton->clicked = [weakIntVal] {
		if (auto sharedVal = weakIntVal.lock()) {
			sharedVal->increment();
		}
	};
	add(incButton);

	decButton = std::make_shared<edui::Button>();
	decButton->setText("");
	decButton->setPosition({1.0f, -24}, {0.5f, 0});
	decButton->setSize({0, 24}, {0.5f, 0});
	decButton->clicked = [weakIntVal] {
		if (auto sharedVal = weakIntVal.lock()) {
			sharedVal->decrement();
		}
	};
	add(decButton);
}

void IntValue::setValue(const int& val) {
	intval->setInt(val);
}

int IntValue::getValue() {
	return intval->getInt();
}
