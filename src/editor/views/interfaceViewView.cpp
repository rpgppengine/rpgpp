#include "views/interfaceViewView.hpp"

#include <memory>

#include "TGUI/Vector2.hpp"
#include "components/resizableCanvasBox.hpp"
#include "editor.hpp"
#include "interfaceView.hpp"
#include "project.hpp"
#include "raylib.h"
#include "uiElement.hpp"
#include "views/worldView.hpp"

InterfaceViewView::InterfaceViewView() {
	canvasBox = std::make_unique<ResizableCanvasBox>("activeEelement", 0, 0, 1, 1, RED);
}

InterfaceViewView::Ptr InterfaceViewView::create() { return std::make_shared<InterfaceViewView>(); }

void InterfaceViewView::setInterfaceView(InterfaceView *ptr) { this->ptr = ptr; }

void InterfaceViewView::setElementAtMouse() {
	if (ptr != nullptr) {
		UIElement *selection = nullptr;
		for (auto &[layer, element] : ptr->getElements()) {
			auto mouse = getMouseWorldPos();
			if (CheckCollisionPointRec(mouse, element->getRect())) {
				selection = element.get();
				canvasBox->updateRec(selection->getRect());

				break;
			}
		}

		this->activeElement = selection;
		if (activeElement == nullptr) {
			canvasBox->updateRec({0, 0, 1, 1});
			onActiveElementChanged.emit(this, "");
		} else {
			onActiveElementChanged.emit(this, activeElement->getName());
		}
	}
}

void InterfaceViewView::drawOverlay() {}

void InterfaceViewView::drawCanvas() {
	ClearBackground(RAYWHITE);

	if (ptr == nullptr) return;

	Font editorFont = Editor::instance->getGui().getFont();

	// draw the UI elements
	const float nameFontSize = 7;
	const float nameSpacing = 0.5f;

	for (auto &item : ptr->getElements()) {
		if (item.second->isVisible()) {
			item.second->draw();
		}

		// outline for element's rect
		auto itemRect = item.second->getRect();
		DrawRectangleLines(static_cast<int>(itemRect.x), static_cast<int>(itemRect.y), static_cast<int>(itemRect.width),
						   static_cast<int>(itemRect.height), Fade(DARKGRAY, 0.7f));

		// draw the name of the element
		auto measureText = MeasureTextEx(editorFont, item.second->getName().c_str(), nameFontSize, nameSpacing);
		Rectangle nameRect = {itemRect.x, itemRect.y, measureText.x + 4, measureText.y + 4};
		DrawRectangleRec(nameRect, Fade(DARKGRAY, 0.7f));
		DrawTextEx(editorFont, item.second->getName().c_str(), {nameRect.x + 2, nameRect.y + 2}, nameFontSize,
				   nameSpacing, WHITE);
	}

	// entities
	auto &ecs = ptr->getCoordinator();
	System &system = ecs.getSystem();
	for (auto &entity : ptr->getEntities()) {
		auto &name = ecs.getEntityName(entity);

		if (ecs.hasComponent<Rectangle>(entity)) {
			system.drawEntity(entity);

			Rectangle rect = ecs.getComponent<Rectangle>(entity);

			DrawRectangleLines(static_cast<int>(rect.x), static_cast<int>(rect.y), static_cast<int>(rect.width),
							   static_cast<int>(rect.height), Fade(DARKGRAY, 0.7f));

			// draw entity name
			auto measureText = MeasureTextEx(editorFont, name.c_str(), nameFontSize, nameSpacing);
			Rectangle nameRect = {rect.x, rect.y, measureText.x + 4, measureText.y + 4};
			DrawRectangleRec(nameRect, Fade(DARKGRAY, 0.7f));
			DrawTextEx(editorFont, name.c_str(), {nameRect.x + 2, nameRect.y + 2}, nameFontSize, nameSpacing, WHITE);
		}
	}

	// draw window borders
	auto windowSize = Editor::instance->getProject()->getProgramSettings().windowSize;

	DrawLine(0, 0, windowSize.x, 0, DARKGRAY);
	DrawLine(0, windowSize.y, windowSize.x, windowSize.y, DARKGRAY);

	DrawLine(0, 0, 0, windowSize.y, DARKGRAY);
	DrawLine(windowSize.x, 0, windowSize.x, windowSize.y, DARKGRAY);

	// draw origin
	drawOrigin();

	if (activeElement != nullptr) {
		canvasBox->draw();
	}
}

bool InterfaceViewView::leftMousePressed(tgui::Vector2f pos) {
	setElementAtMouse();

	const auto &mousePos = getMouseWorldPos();

	if (canvasBox->leftMousePressed(mousePos) && activeElement != nullptr) {
		canvasBox->focused = true;
	}

	return WorldView::leftMousePressed(pos);
}

void InterfaceViewView::leftMouseReleased(tgui::Vector2f pos) {
	if (activeElement != nullptr) {
		activeElement->setRect(canvasBox->getRectangle());
		canvasBox->focused = false;
	}

	WorldView::leftMouseReleased(pos);
}

void InterfaceViewView::mouseMoved(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	if (activeElement != nullptr) {
		canvasBox->mouseMoved(mousePos);
	}

	return WorldView::mouseMoved(pos);
}

void InterfaceViewView::selectElement(const std::string &elementName) {
	if (ptr == nullptr) return;

	for (auto &[layer, element] : ptr->getElements()) {
		if (element->getName() == elementName) {
			activeElement = element.get();
			canvasBox->updateRec(element->getRect());
			break;
		}
	}
}

UIElement *InterfaceViewView::getActiveElement() { return activeElement; }
