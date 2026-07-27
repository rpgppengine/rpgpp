#ifndef _EDUI_HELPER_H
#define _EDUI_HELPER_H

#include <vector>
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

struct CharsBlock {
	int start;
	int end;
};

void loadCodepointsBlock(std::vector<int>& vec, CharsBlock block);

std::vector<int> loadFontCodepoints();

Rectangle paddingRect(Rectangle &rect, float padding);

#endif
