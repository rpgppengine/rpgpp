#include "tween.hpp"

void linear(Tween *tween, float t) {
	// t = 1 - (left / duration)

	*tween->ptr = tween->a + (tween->b - tween->a) * t;
}

// left -= delta time