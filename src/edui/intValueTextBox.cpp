#include "edui/intValueTextBox.hpp"

#include <cstdlib>
#include <memory>
#include <string>

#include "edui/helper.hpp"
#include "edui/textBox.hpp"
#include "edui/widget.hpp"
#include "raylib.h"

using namespace edui;

IntValueTextBox::IntValueTextBox() : TextBox() {
	render = std::make_unique<IntValueTextBoxRender>();
	render->padding = 2.0f;
	render->focusBgColor = RAYWHITE;
	render->as<IntValueTextBoxRender>().vertAlign = VerticalAlignment::TEXT_CENTER;
	focusable = true;

	text = "0";
}

void IntValueTextBox::setInt(const int& val) {
	text = TextFormat("%i", val);
	processText();
}

int IntValueTextBox::getInt() {
	int current = TextToInteger(text.c_str());
	return current;
}

void IntValueTextBox::keyPressed(KeyboardKey key, KeyModifier mod, bool held) {
	bool emit = false;
	int previous = 0;
	if (key == KEY_BACKSPACE || key == KEY_DELETE) {
		emit = true;
		int previous = TextToInteger(text.c_str());
	}

	TextBox::keyPressed(key, mod, held);
	if (emit) {
		int current = TextToInteger(text.c_str());

		if (previous != current) {
			intValueChanged.invoke(previous, current);
		}
	}
}

void IntValueTextBox::charEntered(int codepoint, std::string_view str) {
	if (codepoint >= 48 && codepoint <= 57) {
		int previous = TextToInteger(text.c_str());

		TextBox::charEntered(codepoint, str);
		processText();
		int current = TextToInteger(text.c_str());

		if (previous != current) {
			intValueChanged.invoke(previous, current);
		}
	}
}

void IntValueTextBox::processText() {
	int current = TextToInteger(text.c_str());
	if (current < min) {
		current = min;
	}
	if (current > max) {
		current = max;
	}
	text = TextFormat("%i", current);

	if (cursorIndex > text.size()) {
		cursorIndex = text.size();
	}
	calcCursorRect();
}

void IntValueTextBox::increment() {
	int previous = TextToInteger(text.c_str());
	int current = previous + 1;
	text = TextFormat("%i", current);
	processText();
	current = TextToInteger(text.c_str());

	if (previous != current) {
		intValueChanged.invoke(previous, current);
	}
}

void IntValueTextBox::decrement() {
	int previous = TextToInteger(text.c_str());
	int current = previous - 1;
	text = TextFormat("%i", current);
	processText();
	current = TextToInteger(text.c_str());

	if (previous != current) {
		intValueChanged.invoke(previous, current);
	}
}
