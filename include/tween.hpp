#ifndef _RPGPP_TWEEN_H
#define _RPGPP_TWEEN_H

struct Tween {
	float duration;
	float left;
	float a;
	float b;
	float *ptr;
};

void linear(Tween *tween, float t);

#endif