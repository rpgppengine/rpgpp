#include "edui/helper.hpp"

Rectangle paddingRect(Rectangle& rect, float padding) {
	Rectangle res = rect;
	res.x += padding;
	res.y += padding;
	res.height -= (padding * 2);
	res.width -= (padding * 2);

	return res;
}
