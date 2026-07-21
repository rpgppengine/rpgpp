#include "edui/gui.hpp"
#include "edui/container.hpp"

using namespace edui;

void Gui::update() {
	Rectangle screenRect = {0, 0, static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};

	for (auto& widget : widgets) {
		if (widget->mouseIsInRect()) {
			if (!widget->isContainer) {
				notifyChild(&widget);
			} else {
				widget->as<Container>().notifyChildren(this);
			}
		}

		widget->update();
		widget->calcRect(screenRect);
	}

	if (current != nullptr) {
		int c = GetCharPressed();
		while (c != 0) {
			current->get()->charEntered(static_cast<char>(c));
			c = GetCharPressed();
		}
	}
}

void Gui::draw() {
	for (auto& widget : widgets) {
		widget->draw();
	}
}

void Gui::add(std::shared_ptr<Widget> widget) {
	widget->unfocused();
	widgets.push_back(widget);
}

void Gui::notifyChild(std::shared_ptr<Widget>* widget) {
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
		widget->get()->leftMouseClicked();

		if (current != nullptr) {
			current->get()->unfocused();
		}
		current = widget;
		current->get()->focused();
	}
	if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
		widget->get()->rightMouseClicked();
	}
}
