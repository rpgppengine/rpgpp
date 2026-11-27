#include "animation.hpp"

#include <cstddef>
#include <vector>

#include "basicTypes.hpp"

Animation::Animation() {}

std::vector<IVector> &Animation::getFrames() { return frames; }

void Animation::addFrame(IVector frame) { frames.push_back(frame); }

void Animation::removeFrame(int index) { frames.erase(frames.begin() + index); }

void Animation::insertFrame(int index, IVector frame) { frames.insert(frames.begin() + index, frame); }

size_t Animation::getSize() { return frames.size(); }

IVector Animation::getFrame(int index) { return frames.at(index); }

void Animation::setFrame(int index, IVector frame) { frames[index] = frame; }
