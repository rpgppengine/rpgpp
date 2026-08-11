#ifndef _EDUI_HELPER_H
#define _EDUI_HELPER_H

#include <string>
#include <vector>
#include "raylib.h"

/* Constrain a value between lower nad upper limit. */
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

/* Load an UTF-8 Block. */
void loadCodepointsBlock(std::vector<int>& vec, CharsBlock block);

/* Load all needed UTF-8 blocks. */
std::vector<int> loadFontCodepoints();

/* Calculate a Rectangle with inner padding. */
Rectangle paddingRect(Rectangle &rect, float padding);

/* Draw the text. Draws only part of it and returns true on overflown, otherwise draws the whole text and returns false. */
bool drawOverflownText(Rectangle rect, Font* font, float fontSize, float spacing, const std::string& text, std::string* shownText);

void rectCenter(Rectangle& a, Rectangle* b);

#endif
