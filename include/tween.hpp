#ifndef _RPGPP_TWEEN_H
#define _RPGPP_TWEEN_H

#include <functional>
#include <list>
#include <string>
#include "gamedata.hpp"
#include "sol/sol.hpp"
#include "sol/forward.hpp"

enum class TweenType {
	LINEAR,
	INSINE,
	OUTSINE,
	INOUTSINE,
	INQUAD,
	OUTQUAD,
	INOUTQUAD,
	INCUBIC,
	OUTCUBIC,
	INOUTCUBIC,
	INQUART,
	OUTQUART,
	INOUTQUART,
	INQUINT,
	OUTQUINT,
	INOUTQUINT,
	INEXPO,
	OUTEXPO,
	INOUTEXPO,
	INCIRC,
	OUTCIRC,
	INOUTCIRC,
	INBACK,
	OUTBACK,
	INOUTBACK,
	INELASTIC,
	OUTELASTIC,
	INOUTELASTIC,
	INBOUNCE,
	OUTBOUNCE,
	INOUTBOUNCE
};

#define TWEEN_TYPE_MAX (31)

struct Tween {
	float a;
	float b;
	float *ptr = nullptr;
	float duration;
	TweenType type = TweenType::LINEAR;
	float left;
};

struct TweenLua {
	float a;
	float b;
	sol::userdata source;
	sol::userdata dest;
	std::string key;
	float duration;
	TweenType type = TweenType::LINEAR;
	float left;
	bool empty = true;
};

struct TweenProvider {
	static std::array<std::function<float(float)>, TWEEN_TYPE_MAX> arr;
	static float tweenFunc(TweenType type, float t);
	static void setupTweenFuncs();
	static std::list<Tween> initPropTween(ElementProperty* dest, ElementProperty source, float duration, TweenType type);
};

float linear(float t);

float inSine(float t);
float outSine(float t);
float inOutSine(float t);

float inQuad(float t);
float outQuad(float t);
float inOutQuad(float t);

float inCubic(float t);
float outCubic(float t);
float inOutCubic(float t);

float inQuart(float t);
float outQuart(float t);
float inOutQuart(float t);

float inQuint(float t);
float outQuint(float t);
float inOutQuint(float t);

float inExpo(float t);
float outExpo(float t);
float inOutExpo(float t);

float inCirc(float t);
float outCirc(float t);
float inOutCirc(float t);

float inBack(float t);
float outBack(float t);
float inOutBack(float t);

float inElastic(float t);
float outElastic(float t);
float inOutElastic(float t);

float inBounce(float t);
float outBounce(float t);
float inOutBounce(float t);

#endif
