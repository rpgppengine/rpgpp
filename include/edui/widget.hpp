#ifndef _EDUI_WIDGET_H
#define _EDUI_WIDGET_H

#include <memory>
#include <string>
#include <string_view>

#include "Str.h"
#include "edui/helper.hpp"
#include "edui/signal.hpp"
#include "ini.h"
#include "raylib.h"

namespace edui {
struct Layout {
	float scale = 0.0f;
	int offset = 50;
};

struct Layout2 {
	Layout x = {0, 0};
	Layout y = {0, 0};
	Layout width = {0, 50};
	Layout height = {0, 50};
};

struct Event {
	Vector2 mousePos;
	KeyboardKey key;
	MouseButton mouseButton;
};

struct KeyModifier {
	bool ctrl = false;
	bool shift = false;
	bool alt = false;
	bool numlock = false;
};

typedef mINI::INIMap<std::string> INISection;

struct WidgetRender {
	float padding = 0.0f;
	Color bgColor = RAYWHITE;
	int border = 1;
	Color borderColor = BLACK;
	Color secondaryColor = GRAY;

	Color focusBgColor = LIGHTGRAY;
	Color focusBorderColor = GRAY;

	Color currentBgColor = RAYWHITE;
	Color currentBorderColor = BLACK;

	Font *font = nullptr;

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}

	virtual void applyTheme(const mINI::INIStructure &struc) {
		INISection section = struc.get("Widget");
		bgColor = parseColorString(section.get("BackgroundColor"));
		borderColor = parseColorString(section.get("BorderColor"));
		focusBgColor = parseColorString(section.get("FocusBackgroundColor"));
		focusBorderColor = parseColorString(section.get("FocusBorderColor"));
		secondaryColor = parseColorString(section.get("SecondaryColor"));
	}
};

/* The base class of all widgets in edui. */
struct Widget {
	typedef std::shared_ptr<Widget> Ptr;

	Signal onClicked;
	Signal onRightClicked;
	Signal onReleased;
	Signal onFocused;
	Signal onUnfocused;
	Signal onDeleted;
	Signal onMouseEntered;
	Signal onMouseLeft;

	/* Appearance settings for the Widget. */
	std::unique_ptr<WidgetRender> render;

	bool deleteFlag = false;
	bool deferFlag = false;

	bool visible = true;
	bool focusable = false;
	bool disabled = false;

	bool isContainer = false;
	bool notifiedMouseEnter = false;
	bool isFocused = false;
	bool deleteOnOutsideClick = false;
	bool unfocusOnEnter = true;
	Rectangle outsideClickException = {0, 0, 0, 0};

	Rectangle rect = {0, 0, 0, 0};
	Layout2 layout;
	Vector2 anchor = {0, 0};

	int referId = 0;
	int layerId = 0;
	Str256 translationId = "";

	Widget() = default;
	virtual ~Widget() = default;
	virtual void update() {};
	virtual void draw() {};

	Widget(Widget &other) {
		rect = other.rect;
		layout = other.layout;
		isContainer = other.isContainer;
		render = std::make_unique<WidgetRender>(*other.render.get());
	}

	/* Translate this widget. */
	virtual void translate() {}

	/* Apply a theme to this widget using an INIStructure. */
	virtual void applyTheme(const mINI::INIStructure &struc) { render->applyTheme(struc); }

	virtual void onAdded() {
		unfocused();
		translate();
	}

	/* Calculate the Rectangle for this widget. */
	virtual void calcRect(Rectangle &base) {
		this->rect.x = base.x + ((layout.x.scale * base.width) + layout.x.offset);
		this->rect.y = base.y + ((layout.y.scale * base.height) + layout.y.offset);
		this->rect.width = ((layout.width.scale * base.width) + layout.width.offset);
		this->rect.height = ((layout.height.scale * base.height) + layout.height.offset);

		this->rect.x += static_cast<int>(anchor.x * (base.width - rect.width));
		this->rect.y += static_cast<int>(anchor.y * (base.height - rect.height));
	}

	/* Set position using Layout structs. */
	virtual void setPosition(Layout x, Layout y) {
		this->layout.x = x;
		this->layout.y = y;
	}

	/* Set size using Layout structs. */
	virtual void setSize(Layout width, Layout height) {
		this->layout.width = width;
		this->layout.height = height;
	}

	virtual Rectangle getContentRect() { return rect; }

	/* Calculate the padded rect. */
	virtual Rectangle getPaddingRect() {
		auto content = getContentRect();
		return paddingRect(content, render->padding);
	}

	/* Whether the mouse is inside this widget. */
	bool mouseIsInRect() { return CheckCollisionPointRec(GetMousePosition(), rect); }

	/* Called when the mouse enters this widget. */
	virtual void mouseEntered() {
		onMouseEntered.invoke();
		if (!disabled) {
			render->currentBgColor = render->focusBgColor;
			render->currentBorderColor = render->focusBorderColor;
		}
	}

	/* Called when the mouse leaves this widget. */
	virtual void mouseLeft() {
		onMouseLeft.invoke();
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
	}

	/* Called when the mouse moved while still within this widget. */
	virtual void mouseMoved(Vector2 mousePos, Vector2 relative) {}

	/* Called when the widget has been focused. */
	virtual void focused() {
		if (!disabled) {
			isFocused = true;
			render->currentBgColor = render->focusBgColor;
			render->currentBorderColor = render->focusBorderColor;
			onFocused.invoke();
		}
	}

	/* Called when the widget has been unfocused. Also resets appearance settings to match the current theme. */
	virtual void unfocused() {
		isFocused = false;
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
		onUnfocused.invoke();
	}

	/* Called when the widget has been clicked with the left mouse button. */
	virtual void leftMouseClicked() {
		if (!disabled) {
			onClicked.invoke();
		}
	}

	/* Called when the widget has been clicked with the right mouse button. */
	virtual void rightMouseClicked() {
		if (!disabled) {
			onRightClicked.invoke();
		}
	}

	/* Called when the left mouse button has been released. */
	virtual void leftMouseReleased() { onReleased.invoke(); }

	/* Called when the right mouse button has been released. */
	virtual void rightMouseReleased() {}

	virtual void middleMouseClicked() {}

	virtual void middleMouseReleased() {}

	virtual void charEntered(int codepoint, std::string_view str) {}

	virtual void keyPressed(KeyboardKey key, KeyModifier mod, bool held = false) {}

	virtual void scrolled(float wheelMove) {}

	/* Cast this widget as another type of widget. */
	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}

	/* Clone this widget. */
	virtual Ptr clone() { return std::make_shared<Widget>(*this); }

	/* Mark this widget to be deleted on the next update cycle. */
	virtual void markDelete() { deleteFlag = true; }
};
}  // namespace edui

#endif
