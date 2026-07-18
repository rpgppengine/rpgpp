#include "tween.hpp"

#include <cmath>

#include "raylib.h"

std::array<std::function<float(float)>, TWEEN_TYPE_MAX> TweenProvider::arr;

float TweenProvider::tweenFunc(TweenType type, float t) { return arr[static_cast<int>(type)](t); }

std::list<Tween> TweenProvider::initPropTween(ElementProperty *dest, ElementProperty source, float duration,
											  TweenType type) {
	std::list<Tween> arr = {};

	Tween baseTween = {};
	baseTween.duration = duration;
	baseTween.type = type;

	if (dest->index() == UI_PROP_RECT) {
		Rectangle *destRect = std::get_if<Rectangle>(dest);
		Rectangle sourceRect = std::get<Rectangle>(source);

		Tween x = baseTween;
		x.a = destRect->x;
		x.b = sourceRect.x;
		x.ptr = &destRect->x;
		arr.push_front(x);

		Tween y = baseTween;
		y.a = destRect->y;
		y.b = sourceRect.y;
		y.ptr = &destRect->y;
		arr.push_back(y);

		Tween width = baseTween;
		width.a = destRect->width;
		width.b = sourceRect.width;
		width.ptr = &destRect->width;
		arr.push_back(width);

		Tween height = baseTween;
		height.a = destRect->height;
		height.b = sourceRect.height;
		height.ptr = &destRect->height;
		arr.push_back(height);
	}

	return arr;
}

void TweenProvider::setupTweenFuncs() {
	arr[static_cast<int>(TweenType::LINEAR)] = linear;

	arr[static_cast<int>(TweenType::INSINE)] = inSine;
	arr[static_cast<int>(TweenType::OUTSINE)] = outSine;
	arr[static_cast<int>(TweenType::INOUTSINE)] = inOutSine;

	arr[static_cast<int>(TweenType::INQUAD)] = inQuad;
	arr[static_cast<int>(TweenType::OUTQUAD)] = outQuad;
	arr[static_cast<int>(TweenType::INOUTQUAD)] = inOutQuad;

	arr[static_cast<int>(TweenType::INCUBIC)] = inCubic;
	arr[static_cast<int>(TweenType::OUTCUBIC)] = outCubic;
	arr[static_cast<int>(TweenType::INOUTCUBIC)] = inOutCubic;

	arr[static_cast<int>(TweenType::INQUART)] = inQuart;
	arr[static_cast<int>(TweenType::OUTQUART)] = outQuart;
	arr[static_cast<int>(TweenType::INOUTQUART)] = inOutQuart;

	arr[static_cast<int>(TweenType::INQUINT)] = inQuint;
	arr[static_cast<int>(TweenType::OUTQUINT)] = outQuint;
	arr[static_cast<int>(TweenType::INOUTQUINT)] = inOutQuint;

	arr[static_cast<int>(TweenType::INEXPO)] = inExpo;
	arr[static_cast<int>(TweenType::OUTEXPO)] = outExpo;
	arr[static_cast<int>(TweenType::INOUTEXPO)] = inOutExpo;

	arr[static_cast<int>(TweenType::INCIRC)] = inCirc;
	arr[static_cast<int>(TweenType::OUTCIRC)] = outCirc;
	arr[static_cast<int>(TweenType::INOUTCIRC)] = inOutCirc;

	arr[static_cast<int>(TweenType::INBACK)] = inBack;
	arr[static_cast<int>(TweenType::OUTBACK)] = outBack;
	arr[static_cast<int>(TweenType::INOUTBACK)] = inOutBack;

	arr[static_cast<int>(TweenType::INELASTIC)] = inElastic;
	arr[static_cast<int>(TweenType::OUTELASTIC)] = outElastic;
	arr[static_cast<int>(TweenType::INOUTELASTIC)] = inOutElastic;

	arr[static_cast<int>(TweenType::INBOUNCE)] = inBounce;
	arr[static_cast<int>(TweenType::OUTBOUNCE)] = outBounce;
	arr[static_cast<int>(TweenType::INOUTBOUNCE)] = inOutBounce;
}

float linear(float t) { return t; }

float inSine(float t) { return 1 - std::cos((t * PI) / 2); }

float outSine(float t) { return std::sin((t * PI) / 2); }

float inOutSine(float t) { return -(std::cos(PI * t) - 1) / 2; }

float inQuad(float t) { return t * t; }

float outQuad(float t) { return 1 - (1 - t) * (1 - t); }

float inOutQuad(float t) { return t < 0.5f ? 2 * t * t : 1 - std::pow(-2 * t + 2, 2) / 2; }

float inCubic(float t) { return t * t * t; }

float outCubic(float t) { return 1 - std::pow(1 - t, 3); }

float inOutCubic(float t) { return t < 0.5f ? 4 * t * t * t : 1 - std::pow(-2 * t + 2, 3) / 2; }

float inQuart(float t) { return t * t * t * t; }

float outQuart(float t) { return 1 - std::pow(1 - t, 4); }

float inOutQuart(float t) { return t < 0.5f ? 8 * t * t * t * t : 1 - std::pow(-2 * t + 2, 4) / 2; }

float inQuint(float t) { return t * t * t * t * t; }

float outQuint(float t) { return 1 - std::pow(1 - t, 5); }

float inOutQuint(float t) { return t < 0.5f ? 16.0f * t * t * t * t * t : 1 - std::pow(-2 * t + 2, 5) / 2.0f; }

float inExpo(float t) { return t == 0 ? 0 : std::pow(2, 10 * t - 10); }

float outExpo(float t) { return t == 1 ? 1 : 1 - std::pow(2, -10 * t); }

float inOutExpo(float t) {
	return t == 0 ? 0 : t == 1 ? 1 : t < 0.5f ? std::pow(2, 20 * t - 10) / 2 : (2 - std::pow(2, -20 * t + 10)) / 2;
}

float inCirc(float t) { return 1 - std::sqrt(1 - std::pow(t, 2)); }

float outCirc(float t) { return std::sqrt(1 - std::pow(t - 1, 2)); }

float inOutCirc(float t) {
	return t < 0.5f ? (1 - std::sqrt(1 - std::pow(2 * t, 2))) / 2 : (std::sqrt(1 - std::pow(-2 * t + 2, 2)) + 1) / 2;
}

float inBack(float t) {
	const float c1 = 1.70158;
	const float c2 = c1 + 1.0f;

	return c2 * t * t * t - c1 * t * t;
}

float outBack(float t) {
	const float c1 = 1.70158;
	const float c2 = c1 + 1.0f;

	return 1 + c2 * std::pow(t - 1, 3) + c1 * std::pow(t - 1, 2);
}

float inOutBack(float t) {
	const float c1 = 1.70158;
	const float c2 = c1 + 1.0f;

	return t < 0.5f ? (std::pow(2 * t, 2) * ((c2 + 1) * 2 * t - c2)) / 2
					: (std::pow(2 * t - 2, 2) * ((c2 + 1) * (t * 2 - 2) + c2) + 2) / 2;
}

float inElastic(float t) {
	const float c4 = (2 * PI) / 3;

	return t == 0 ? 0 : t == 1 ? 1 : -std::pow(2, 10 * t - 10) * std::sin((t * 10 - 10.75f) * c4);
}

float outElastic(float t) {
	const float c4 = (2 * PI) / 3;

	return t == 0 ? 0 : t == 1 ? 1 : -std::pow(2, -10 * t) * std::sin((t * 10 - 0.75f) * c4) + 1;
}

float inOutElastic(float t) {
	const float c5 = (2 * PI) / 4.5f;

	return t == 0	  ? 0
		   : t == 1	  ? 1
		   : t < 0.5f ? -std::pow(2, 20 * t - 10) * std::sin((t * 20 - 11.125f) * c5) / 2
					  : std::pow(2, -20 * t + 10) * std::sin((t * 20 - 11.125f) * c5) / 2 + 1;
}

float inBounce(float t) { return 1 - outBounce(t - 1); }

float outBounce(float t) {
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < 1.0f / d1) {
		return n1 * t * t;
	} else if (t < 2.0f / d1) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	} else if (t < 2.5f / d1) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	} else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float inOutBounce(float t) { return t < 0.5f ? (1 - outBounce(1 - 2 * t)) / 2 : (1 + outBounce(2 * t - 1)) / 2; }
