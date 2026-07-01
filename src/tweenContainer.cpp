#include "tweenContainer.hpp"

#include "raylib.h"
#include "tween.hpp"

TweenContainer::TweenContainer() { tweens = {}; }

void TweenContainer::addTween(Tween tween) {
	if (size < MAX_TWEENS) {
		for (int i = 0; i < MAX_TWEENS; i++) {
			if (tweens[i].ptr == nullptr) {
				tween.left = tween.duration;
				*tween.ptr = tween.a;
				tweens[i] = tween;
				size++;
				break;
			}
		}
	}
}

void TweenContainer::removeTween(int index) {
	tweens[index] = {};
	tweens[index].ptr = nullptr;
	size--;
}

void TweenContainer::addLuaTween(TweenLua tween) {
	tween.empty = false;
	tween.left = tween.duration;

	if (size < MAX_TWEENS) {
		for (int i = 0; i < MAX_TWEENS; i++) {
			if (luaTweens[i].empty) {
				luaTweens[i] = tween;
				sizeLua++;
				break;
			}
		}
	}
}

void TweenContainer::removeLuaTween(int index) {
	luaTweens[index] = {};
	luaTweens[index].empty = true;
	sizeLua--;
}

void TweenContainer::update() {
	for (int i = 0; i < MAX_TWEENS; i++) {
		if (tweens[i].ptr != nullptr) {
			auto &tween = tweens[i];
			float t = 1 - (tween.left / tween.duration);
			if (static_cast<int>(*tween.ptr) != static_cast<int>(tween.b)) {
				float val = TweenProvider::tweenFunc(tween.type, t);

				if (tween.b > tween.a) {
					*tween.ptr = tween.a + ((tween.b - tween.a) * val);
				} else {
					*tween.ptr = tween.a - ((tween.a - tween.b) * val);
				}

				if (val >= 1.0f) {
					removeTween(i);
					return;
				}

				tween.left -= GetFrameTime();
			} else {
				removeTween(i);
			}
		}
	}

	// lua tweens
	for (int i = 0; i < MAX_TWEENS; i++) {
		if (!luaTweens[i].empty) {
			auto &tween = luaTweens[i];
			float t = 1 - (tween.left / tween.duration);
			if (t < 1) {
				float val = TweenProvider::tweenFunc(tween.type, t);

				if (tween.b > tween.a) {
					float newVal = tween.a + ((tween.b - tween.a) * val);
					tween.dest.set(tween.key, newVal);
				} else {
					float newVal = tween.a - ((tween.a - tween.b) * val);
					tween.dest.set(tween.key, newVal);
				}

				tween.left -= GetFrameTime();
			} else {
				removeLuaTween(i);
			}
		}
	}
}

int TweenContainer::getSize() {
	return size;
}

void TweenContainer::abandonLua() {
	for (int i = 0; i < MAX_TWEENS; i++) {
		if (luaTweens[i].source.valid()) {
			luaTweens[i].source.abandon();
		}
		if (luaTweens[i].dest.valid()) {
			luaTweens[i].dest.abandon();
		}
	}
}
