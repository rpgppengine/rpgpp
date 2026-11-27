#ifndef _RPGPP_ANIMATION_H
#define _RPGPP_ANIMATION_H

#include <cstddef>
#include <vector>

#include "basicTypes.hpp"

class Animation {
public:
	Animation();

	std::vector<IVector> &getFrames();
	void addFrame(IVector frame);
	void removeFrame(int index);
	void insertFrame(int index, IVector frame);
	size_t getSize();
	IVector getFrame(int index);
	void setFrame(int index, IVector frame);

	template <class Archive>
	void serialize(Archive &a) {
		a(CEREAL_NVP(frames));
	}

private:
	std::vector<IVector> frames;
};

#endif
