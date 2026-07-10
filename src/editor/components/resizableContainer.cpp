#include "components/resizableContainer.hpp"

#include <memory>

#include "TGUI/Backend/Renderer/BackendRenderTarget.hpp"
#include "TGUI/Layout.hpp"
#include "TGUI/Vector2.hpp"
#include "TGUI/Widget.hpp"
#include "TGUI/Widgets/Group.hpp"
#include "raylib.h"

bool bounded(int value, int min, int max) { return value >= min && value <= max; }

ResizableContainer::ResizableContainer(const tgui::Layout2d &size, const tgui::Layout2d &position) : tgui::Group() {
	this->setSize(size);
	this->setPosition(position);
}

void ResizableContainer::setMaxResizeWidth(int width) { maxResizeWidth = width; }
int ResizableContainer::getMaxResizeWidth() { return maxResizeWidth; }
void ResizableContainer::setMaxResizeHeight(int height) { maxResizeHeight = height; }
int ResizableContainer::getMaxResizeHeight() { return maxResizeHeight; }
void ResizableContainer::setMinResizeWidth(int width) { minResizeWidth = width; }
void ResizableContainer::setMinResizeHeight(int height) { minResizeHeight = height; }
int ResizableContainer::getMinResizeWidth() { return minResizeWidth; }
int ResizableContainer::getMinResizeHeight() { return minResizeHeight; }
void ResizableContainer::setGrabberSize(int size) { grabberSize = size; }
int ResizableContainer::getGrabberSize() { return grabberSize; }

void ResizableContainer::enableResize(ResizeDirectionC direction) { resizeFlags |= static_cast<char>(direction); }

void ResizableContainer::disableResize(ResizeDirectionC direction) { resizeFlags &= ~static_cast<char>(direction); }

bool ResizableContainer::isResizable(ResizeDirectionC direction) { return resizeFlags & static_cast<char>(direction); }

ResizableContainer::Ptr ResizableContainer::create(const tgui::Layout2d &size, const tgui::Layout2d &position) {
	return std::make_shared<ResizableContainer>(size, position);
}

ResizableContainer::Ptr ResizableContainer::copy(ResizableContainer::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<ResizableContainer>(widget->clone());
	}
	return nullptr;
}

tgui::Widget::Ptr ResizableContainer::clone() const { return std::make_shared<ResizableContainer>(*this); }

bool ResizableContainer::inEnabledGrabber(ResizeDirectionC direction, tgui::Vector2f absolutePos) {
	if (!isResizable(direction)) return false;

	switch (direction) {
		case ResizeDirectionC::LEFT:
			return bounded(absolutePos.x, 0, grabberSize) and bounded(absolutePos.y, 0, getSize().y);
		case ResizeDirectionC::RIGHT:
			return bounded(absolutePos.x, getSize().x - grabberSize, getSize().x) and
				   bounded(absolutePos.y, 0, getSize().y);
		case ResizeDirectionC::TOP:
			return bounded(absolutePos.y, 0, grabberSize) and bounded(absolutePos.x, 0, getSize().x);
		case ResizeDirectionC::BOTTOM:
			return bounded(absolutePos.y, getSize().y - grabberSize, getSize().y) and
				   bounded(absolutePos.x, 0, getSize().x);
		default:
			return false;
	}
}

bool ResizableContainer::leftMousePressed(tgui::Vector2f pos) {
	auto absolutePos = pos - getPosition();
	startMousePos = absolutePos;
	startSize = getSize();
	startPosition = getPosition();

	if (inEnabledGrabber(ResizeDirectionC::LEFT, absolutePos)) {
		grabbingFlag = static_cast<char>(ResizeDirectionC::LEFT);
	} else if (inEnabledGrabber(ResizeDirectionC::RIGHT, absolutePos)) {
		grabbingFlag = static_cast<char>(ResizeDirectionC::RIGHT);
	} else if (inEnabledGrabber(ResizeDirectionC::TOP, absolutePos)) {
		grabbingFlag = static_cast<char>(ResizeDirectionC::TOP);
	} else if (inEnabledGrabber(ResizeDirectionC::BOTTOM, absolutePos)) {
		grabbingFlag = static_cast<char>(ResizeDirectionC::BOTTOM);
	}

	return tgui::Group::leftMousePressed(pos);
}

void ResizableContainer::manualMouseMoved(tgui::Vector2f pos) {
	auto absolutePos = pos - getPosition();

	if (inEnabledGrabber(ResizeDirectionC::LEFT, absolutePos)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
		cursorModified = true;
	} else if (inEnabledGrabber(ResizeDirectionC::RIGHT, absolutePos)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_EW);
		cursorModified = true;
	} else if (inEnabledGrabber(ResizeDirectionC::TOP, absolutePos)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
		cursorModified = true;
	} else if (inEnabledGrabber(ResizeDirectionC::BOTTOM, absolutePos)) {
		SetMouseCursor(MOUSE_CURSOR_RESIZE_NS);
		cursorModified = true;
	} else if (cursorModified) {
		SetMouseCursor(MOUSE_CURSOR_DEFAULT);
		cursorModified = false;
	}

	tgui::Vector2f deltaMousePos = absolutePos - startMousePos;
	if (grabbingFlag == static_cast<char>(ResizeDirectionC::LEFT)) {
		float newW = startSize.x.getValue() - deltaMousePos.x;
		newW = std::clamp(newW, static_cast<float>(minResizeWidth), static_cast<float>(maxResizeWidth));
		setSize(newW, startSize.y);
		setPosition(startPosition.x + deltaMousePos.x, startPosition.y);
	} else if (grabbingFlag == static_cast<char>(ResizeDirectionC::RIGHT)) {
		float newW = startSize.x.getValue() + deltaMousePos.x;
		newW = std::clamp(newW, static_cast<float>(minResizeWidth), static_cast<float>(maxResizeWidth));
		setSize(newW, startSize.y);
	} else if (grabbingFlag == static_cast<char>(ResizeDirectionC::TOP)) {
		float newH = startSize.y.getValue() - deltaMousePos.y;
		newH = std::clamp(newH, static_cast<float>(minResizeHeight), static_cast<float>(maxResizeHeight));
		setSize(startSize.x, newH);
		setPosition(startPosition.x, startPosition.y + deltaMousePos.y);
	} else if (grabbingFlag == static_cast<char>(ResizeDirectionC::BOTTOM)) {
		float newH = startSize.y.getValue() + deltaMousePos.y;
		newH = std::clamp(newH, static_cast<float>(minResizeHeight), static_cast<float>(maxResizeHeight));
		setSize(startSize.x, newH);
	}
	onResize.emit(this, tgui::Layout2d{getSize().x - startSize.x, getSize().y - startSize.y});
}
void ResizableContainer::mouseMoved(tgui::Vector2f pos) {
	tgui::Group::mouseMoved(pos);
	if (!useExternalMouseEvent) {
		manualMouseMoved(pos);
	}
}

void ResizableContainer::manualLeftMouseReleased(tgui::Vector2f pos) { grabbingFlag = 0; }

void ResizableContainer::leftMouseReleased(tgui::Vector2f pos) {
	tgui::Group::leftMouseReleased(pos);
	if (!useExternalMouseEvent) {
		manualLeftMouseReleased(pos);
	}
}
