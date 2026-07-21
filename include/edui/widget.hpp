#ifndef _EDUI_WIDGET_H
#define _EDUI_WIDGET_H

#include <cstdio>
#include <memory>

#include "raylib.h"

namespace edui {
struct Layout {
	float scale = 0.0f;
	int offset = 50;
};

struct Layout2 {
	Layout x;
	Layout y;
	Layout width;
	Layout height;
};

struct Event {
	Vector2 mousePos;
	KeyboardKey key;
	MouseButton mouseButton;
};

struct WidgetRender {
	float padding = 0.0f;
	Color bgColor = RAYWHITE;
	int border = 1;
	Color borderColor = BLACK;

	Color focusBgColor = RAYWHITE;
	Color focusBorderColor = GRAY;

	Color currentBgColor = RAYWHITE;
	Color currentBorderColor = BLACK;

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}
};

struct Widget {
	typedef std::shared_ptr<Widget> Ptr;

	std::unique_ptr<WidgetRender> render;

	bool isContainer = false;

	Rectangle rect;
	Layout2 layout;

	Widget() = default;
	virtual ~Widget() = default;
	virtual void update() {};
	virtual void draw() = 0;

	void calcRect(Rectangle &base) {
		this->rect.x = base.x + ((layout.x.scale * base.width) + layout.x.offset);
		this->rect.y = base.y + ((layout.y.scale * base.height) + layout.y.offset);
		this->rect.width = ((layout.width.scale * base.width) + layout.width.offset);
		this->rect.height = ((layout.height.scale * base.height) + layout.height.offset);
	}

	void setPosition(Layout x, Layout y) {
		this->layout.x = x;
		this->layout.y = y;
	}

	void setSize(Layout width, Layout height) {
		this->layout.width = width;
		this->layout.height = height;
	}

	Rectangle getPaddingRect() {
		Rectangle res = rect;
		res.x += render->padding;
		res.y += render->padding;
		res.height -= (render->padding * 2);
		res.width -= (render->padding * 2);

		return res;
	}

	bool mouseIsInRect() {
		return CheckCollisionPointRec(GetMousePosition(), rect);
	}

	virtual void focused() {
		render->currentBgColor = render->focusBgColor;
		render->currentBorderColor = render->focusBorderColor;
	}

	virtual void unfocused() {
		render->currentBgColor = render->bgColor;
		render->currentBorderColor = render->borderColor;
	}

	virtual void leftMouseClicked() {
		printf("left mouse.. \n");
	}

	virtual void rightMouseClicked() {
		printf("right mouse.. \n");
	}

	virtual void charEntered(char c) {
		printf("%c \n", c);
	}

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}
};
}  // namespace edui

#endif
