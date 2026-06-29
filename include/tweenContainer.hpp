#ifndef _RPGPP_TWEENCONTAINER_H
#define _RPGPP_TWEENCONTAINER_H

#include <array>

#include "tween.hpp"

#define MAX_TWEENS (5)

class TweenContainer {
private:
	std::array<Tween, MAX_TWEENS> tweens;
	int size = 0;

public:
	TweenContainer();
	void addTween(Tween tween);
	void removeTween(int index);
	void update();
};

#endif