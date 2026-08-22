#ifndef _EDUI_WIDGET_H
#define _EDUI_WIDGET_H

#include <cstdio>
#include <memory>
#include <string_view>

#include "edui/helper.hpp"
#include "edui/signal.hpp"
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
};

struct Widget {
	typedef std::shared_ptr<Widget> Ptr;

	Signal onClicked;
	Signal onRightClicked;
	Signal onFocused;
	Signal onUnfocused;
	Signal onDeleted;
	Signal onMouseEntered;
	Signal onMouseLeft;

	std::unique_ptr<WidgetRender> render;

	bool deleteFlag = false;
	bool deferFlag = false;

	bool visible = true;
	bool focusable = false;
	bool isContainer = false;
	bool notifiedMouseEnter = false;
	bool isFocused = false;
	bool deleteOnOutsideClick = false;

	Rectangle rect = {0, 0, 0, 0};
	Layout2 layout;
	Vector2 anchor = {0, 0};

	int referId = 0;

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

	virtual void calcRect(Rectangle &base) {
		this->rect.x = base.x + ((layout.x.scale * base.width) + layout.x.offset);
		this->rect.y = base.y + ((layout.y.scale * base.height) + layout.y.offset);
		this->rect.width = ((layout.width.scale * base.width) + layout.width.offset);
		this->rect.height = ((layout.height.scale * base.height) + layout.height.offset);

		this->rect.x += anchor.x * (base.width - rect.width);
		this->rect.y += anchor.y * (base.height - rect.height);
	}

	virtual void setPosition(Layout x, Layout y) {
		this->layout.x = x;
		this->layout.y = y;
	}

	virtual void setSize(Layout width, Layout height) {
		this->layout.width = width;
		this->layout.height = height;
	}

	virtual Rectangle getContentRect() { return rect; }

	virtual Rectangle getPaddingRect() {
		auto content = getContentRect();
		return paddingRect(content, render->padding);
	}

	bool mouseIsInRect() { return CheckCollisionPointRec(GetMousePosition(), rect); }

	virtual void mouseEntered() {
		onMouseEntered.invoke();
		render->currentBgColor = render->focusBgColor;
		render->currentBorderColor = render->focusBorderColor;
	}

	virtual void mouseLeft() {
		onMouseLeft.invoke();
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
	}

	virtual void mouseMoved(Vector2 mousePos, Vector2 relative) {}

	virtual void focused() {
		isFocused = true;
		render->currentBgColor = render->focusBgColor;
		render->currentBorderColor = render->focusBorderColor;
		onFocused.invoke();
	}

	virtual void unfocused() {
		isFocused = false;
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
		onUnfocused.invoke();
	}

	virtual void leftMouseClicked() { onClicked.invoke(); }

	virtual void rightMouseClicked() { onRightClicked.invoke(); }

	virtual void leftMouseReleased() {}

	virtual void rightMouseReleased() {}

	virtual void middleMouseClicked() {}

	virtual void middleMouseReleased() {}

	virtual void charEntered(int codepoint, std::string_view str) {}

	virtual void keyPressed(KeyboardKey key, KeyModifier mod, bool held = false) {}

	virtual void scrolled(float wheelMove) {}

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}

	virtual Ptr clone() { return std::make_shared<Widget>(*this); }

	virtual void markDelete() { deleteFlag = true; }
};
}  // namespace edui

#endif
