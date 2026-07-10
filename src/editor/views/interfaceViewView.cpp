#include "views/interfaceViewView.hpp"

#include <memory>

#include "TGUI/Vector2.hpp"
#include "actions/action.hpp"
#include "components/resizableCanvasBox.hpp"
#include "drawHelper.hpp"
#include "editor.hpp"
#include "interfaceView.hpp"
#include "project.hpp"
#include "raylib.h"
#include "screens/projectScreen.hpp"
#include "views/worldView.hpp"

bool RectanglesEqual(Rectangle rec1, Rectangle rec2) {
	return (rec1.x == rec2.x) &&
	(rec1.y && rec2.y) &&
	(rec1.width && rec2.width) &&
	(rec1.height && rec2.height);
}

InterfaceViewView::InterfaceViewView() {
	canvasBox = std::make_unique<ResizableCanvasBox>("activeEelement", 0, 0, 1, 1, RED);
}

InterfaceViewView::Ptr InterfaceViewView::create() { return std::make_shared<InterfaceViewView>(); }

void InterfaceViewView::setInterfaceView(InterfaceView *ptr) { this->ptr = ptr; }

void InterfaceViewView::setElementAtMouse() {
	if (ptr != nullptr) {
		auto mouse = getMouseWorldPos();
		ElementIndex selection = MAX_ELEMENTS;

		for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
			auto element = ptr->getElement(i);
			if (element != nullptr) {
				auto &rect = std::get<Rectangle>(element->props["rect"]);
				if (CheckCollisionPointRec(mouse, rect)) {
					selection = i;
					canvasBox->updateRec(rect);

					break;
				}
			}
		}

		this->activeElement = selection;
		if (selection == MAX_ELEMENTS) {
			canvasBox->updateRec({0, 0, 1, 1});
			onActiveElementChanged.emit(this, "");
			onActiveEntityChanged.emit(this, MAX_ELEMENTS);
		} else {
			onActiveElementChanged.emit(this, ptr->getEntityName(selection));
			onActiveEntityChanged.emit(this, activeElement);
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
	for (ElementIndex i = 0; i < MAX_ELEMENTS; i++) {
		auto element = ptr->getElement(i);
		if (element != nullptr) {
			auto &name = ptr->getEntityName(i);

			ptr->drawEntity(i);

			// draw element rect
			Rectangle rect = std::get<Rectangle>(element->props["rect"]);
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

	if (activeElement != MAX_ELEMENTS) {
		canvasBox->draw();
	}
}

bool InterfaceViewView::leftMousePressed(tgui::Vector2f pos) {
	setElementAtMouse();

	const auto &mousePos = getMouseWorldPos();

	if (canvasBox->leftMousePressed(mousePos) && activeElement != MAX_ELEMENTS) {
		canvasBox->focused = true;
	}

	return WorldView::leftMousePressed(pos);
}

void InterfaceViewView::leftMouseReleased(tgui::Vector2f pos) {
	if (activeElement != MAX_ELEMENTS) {
		auto elementPtr = ptr->getElement(activeElement);

		if (elementPtr != nullptr) {
			Rectangle &elementRect = std::get<Rectangle>(elementPtr->props["rect"]);
			Rectangle oldRect = elementRect;

			elementRect = canvasBox->getRectangle();
			canvasBox->focused = false;

			if (!RectanglesEqual(oldRect, elementRect)) {
				auto action = std::make_unique<Action>();
				action->onAction = [elementPtr, elementRect, this] {
					Rectangle* ptr = std::get_if<Rectangle>(&elementPtr->props["rect"]);
					*ptr = elementRect;

					visitProps(this->ptr->getEntityName(activeElement));

					canvasBox->updateRec(elementRect);
				};
				action->onUndo = [elementPtr, oldRect, this] {
					Rectangle* ptr = std::get_if<Rectangle>(&elementPtr->props["rect"]);
					*ptr = oldRect;

					visitProps(this->ptr->getEntityName(activeElement));

					canvasBox->updateRec(oldRect);
				};

				auto screen = aurora::downcast<screens::ProjectScreen *>(Editor::instance->getGui().currentScreen.get());
				screen->getCurrentFile().getView().pushAction(std::move(action));
			}
		}
	}

	WorldView::leftMouseReleased(pos);
}

void InterfaceViewView::mouseMoved(tgui::Vector2f pos) {
	const auto &mousePos = getMouseWorldPos();

	if (activeElement != MAX_ELEMENTS) {
		canvasBox->mouseMoved(mousePos);
	}

	return WorldView::mouseMoved(pos);
}

void InterfaceViewView::selectElement(const std::string &elementName) {
	if (ptr == nullptr) return;

	auto elementId = ptr->findByName(elementName);
	if (elementId != MAX_ELEMENTS) {
		activeElement = elementId;

		auto element = ptr->getElement(elementId);

		if (element != nullptr) {
			Rectangle rect = std::get<Rectangle>(ptr->getElement(elementId)->props["rect"]);
			canvasBox->updateRec(rect);
		}
	}
}

void InterfaceViewView::visitProps(const std::string &title) {
	if (propBox != nullptr && visitor != nullptr) return;
	propBox->clear();

	visitor->box = propBox;
	visitor->view = ptr;

	auto element = ptr->getElement(title);
	visitor->element = element;
	if (element == nullptr) return;
	for (auto &[title, variant] : element->props) {
		visitor->key = title;
		std::visit(*visitor, variant);
	}
}
