#ifndef _EDUI_HELPER_H
#define _EDUI_HELPER_H

#include "raylib.h"

template <class T>
const T &constrain(const T &x, const T &a, const T &b) {
	if (x < a) {
		return a;
	} else if (b < x) {
		return b;
	} else
		return x;
}

Rectangle paddingRect(Rectangle &rect, float padding);

#endif
