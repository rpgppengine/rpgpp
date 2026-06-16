#include "views/interfaceViewView.hpp"

#include <memory>

#include "TGUI/Vector2.hpp"
#include "component.hpp"
#include "components/resizableCanvasBox.hpp"
#include "drawHelper.hpp"
#include "editor.hpp"
#include "entity.hpp"
#include "interfaceView.hpp"
#include "project.hpp"
#include "raylib.h"
#include "views/worldView.hpp"

InterfaceViewView::InterfaceViewView() {
	canvasBox = std::make_unique<ResizableCanvasBox>("activeEelement", 0, 0, 1, 1, RED);
}

InterfaceViewView::Ptr InterfaceViewView::create() { return std::make_shared<InterfaceViewView>(); }

void InterfaceViewView::setInterfaceView(InterfaceView *ptr) { this->ptr = ptr; }

void InterfaceViewView::setElementAtMouse() {
	if (ptr != nullptr) {
		EntityID selection = MAX_ENTITIES;
		auto mouse = getMouseWorldPos();
		for (auto &entity : ptr->getEntities()) {
			if (ptr->getCoordinator().hasComponent<Rectangle>(entity)) {
				auto &rect = ptr->getCoordinator().getComponent<Rectangle>(entity);
				if (CheckCollisionPointRec(mouse, rect)) {
					selection = entity;
					canvasBox->updateRec(rect);

					break;
				}
			}
		}

		this->activeEntity = selection;
		if (selection == MAX_ENTITIES) {
			canvasBox->updateRec({0, 0, 1, 1});
			onActiveElementChanged.emit(this, "");
			onActiveEntityChanged.emit(this, MAX_ENTITIES);
		} else {
			onActiveElementChanged.emit(this, ptr->getCoordinator().getEntityName(activeEntity));
			onActiveEntityChanged.emit(this, activeEntity);
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

	// entities
	auto &ecs = ptr->getCoordinator();
	System &system = ecs.getSystem();
	for (auto &entity : ptr->getEntities()) {
		auto &name = ecs.getEntityName(entity);

		if (ecs.hasComponent<Rectangle>(entity)) {
			system.drawEntity(entity);

			// draw entity's rect
			Rectangle rect = ecs.getComponent<Rectangle>(entity);
			DrawRectangleLinesPro(rect, Fade(DARKGRAY, 0.7f));

			// draw entity name
			auto measureText = MeasureTextEx(editorFont, name.c_str(), nameFontSize, nameSpacing);
			Rectangle nameRect = {rect.x, rect.y, measureText.x + 4, measureText.y + 4};
			DrawRectangleRec(nameRect, Fade(DARKGRAY, 0.7f));
			DrawTextEx(editorFont, name.c_str(), {nameRect.x + 2, nameRect.y + 2}, nameFontSize, nameSpacing, WHITE);
		}
	}

	// draw window borders
	auto windowSize = Editor::instance->getProject()->getProgramSettings().windowSize;

	Rectangle windowSizeRec = {0, 0, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y)};
	DrawRectangleLinesPro(windowSizeRec, DARKGRAY);

	// draw origin
	drawOrigin();

	if (activeEntity != MAX_ENTITIES) {
		canvasBox->draw();
	}
}

bool InterfaceViewView::leftMousePressed(tgui::Vector2f pos) {
	setElementAtMouse();

	const auto &mousePos = getMouseWorldPos();

	if (canvasBox->leftMousePressed(mousePos) && activeEntity != MAX_ENTITIES) {
		canvasBox->focused = true;
	}

	return WorldView::leftMousePressed(pos);
}

void InterfaceViewView::leftMouseReleased(tgui::Vector2f pos) {
	if (activeEntity != MAX_ENTITIES) {
		if (ptr->getCoordinator().hasComponent<Rectangle>(activeEntity)) {
			auto &rect = ptr->getCoordinator().getComponent<Rectangle>(activeEntity);
			rect = canvasBox->getRectangle();
			canvasBox->focused = false;

			if (propBox != nullptr && visitor != nullptr) {
				propBox->clear();

				auto &ecs = ptr->getCoordinator();

				auto set = ecs.getEntityComponents(activeEntity);
				for (auto &name : set) {
					auto componentVariant = ecs.getComponentVariant(activeEntity, name);
					visitor->componentVisit(componentVariant, propBox);
				}
			}
		}
	}

	WorldView::leftMouseReleased(pos);
}

void InterfaceViewView::mouseMoved(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	if (activeEntity != MAX_ENTITIES) {
		canvasBox->mouseMoved(mousePos);
	}

	return WorldView::mouseMoved(pos);
}

void InterfaceViewView::selectElement(const std::string &elementName) {
	if (ptr == nullptr) return;

	auto &entityManager = ptr->getCoordinator().getEntityManager();
	auto entity = entityManager.findName(elementName);
	if (entity != MAX_ENTITIES) {
		if (ptr->getCoordinator().hasComponent<Rectangle>(entity)) {
			activeEntity = entity;
			canvasBox->updateRec(ptr->getCoordinator().getComponent<Rectangle>(entity));
		}
	}
}
