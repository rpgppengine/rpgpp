#ifndef _EDUI_INTVALUETEXTBOX_H
#define _EDUI_INTVALUETEXTBOX_H

#include <memory>

#include "edui/signal.hpp"
#include "edui/textBox.hpp"
#include "edui/widget.hpp"

namespace edui {
struct IntValueTextBoxRender : public TextBoxRender {};

struct IntValueTextBox : public TextBox {
	typedef std::shared_ptr<IntValueTextBox> Ptr;

	int min = 0;
	int max = 100;

	SignalT2<int, int> valueChanged;

	IntValueTextBox();

	void keyPressed(KeyboardKey key, KeyModifier mod, bool held) override;
	void charEntered(int codepoint, std::string_view str) override;

	void increment();
	void decrement();

private:
	void processText();
};
}  // namespace edui

#endif
