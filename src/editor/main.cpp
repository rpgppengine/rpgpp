#include <memory>
#include "editor.hpp"
#include "edui/colorRect.hpp"
#include "edui/gui.hpp"
#include "edui/container.hpp"
#include "edui/horizontalContainer.hpp"
#include "edui/verticalContainer.hpp"
#include "services/editorGuiService.hpp"

#define SOL_EXCEPTIONS_SAFE_PROPAGATION

int main() {
	/*
	const auto editor = std::make_unique<Editor>();
	auto &gui = editor->getGui();

	gui.init();
	gui.uiLoop();
	editor->unload();
	*/

	edui::Gui gui;

	auto colorRect = std::make_shared<edui::ColorRect>();
	colorRect->layout.x = {0.5f, 0};
	colorRect->layout.y = {0, 0};
	colorRect->layout.width = {0, 50};
	colorRect->layout.height = {0.5f, 0};
	gui.add(colorRect);

	//auto container = std::make_shared<edui::Container>();
	auto container = std::make_shared<edui::HorizontalContainer>();
	container->setPosition({0, 0}, {0, 0});
	container->setSize({0.5f, 0}, {0.5f, 0});
	container->render->padding = 4.0f;
	container->render->as<edui::HorizontalContainerRender>().space = 4;
	gui.add(container);

	auto newRect = std::make_shared<edui::ColorRect>();
	newRect->render->as<edui::ColorRectRender>().bgColor = MAGENTA;
	newRect->setPosition({0, 0}, {0, 0});
	newRect->setSize({0.25f, 50}, {1.0f, 50});
	container->add(newRect);

	auto newRect2 = newRect->clone();
	//newRect2->render->as<edui::ColorRectRender>().bgColor = GREEN;
	//newRect2->setPosition({0, 0}, {0, 0});
	//newRect2->setSize({0.25, 50}, {1, 50});
	container->add(newRect2);

	auto vert = std::make_shared<edui::VerticalContainer>();
	vert->setPosition({0, 0}, {0.5f, 0});
	vert->setSize({0.5f, 0}, {0.5f, 0});
	vert->render->padding = 4.0f;
	vert->scissorRect = {0, 0, 0, 280};
	gui.add(vert);

	auto newRect3 = std::make_shared<edui::ColorRect>();
	newRect3->setSize({0, 50}, {0, 50});
	newRect3->render->focusBgColor = GRAY;
	vert->add(newRect3);

	auto newRect4 = newRect3->clone();
	vert->add(newRect4);

	for (int i = 0; i < 10; i++) {
		auto clonedRect = newRect3->clone();
		vert->add(clonedRect);
	}

	for (int i = 0; i < 10; i++) {
		auto clonedRect = newRect->clone();
		container->add(clonedRect);
	}

	InitWindow(800, 450, "raylib example - basic window");

    while (!WindowShouldClose())
    {
    	gui.update();

        BeginDrawing();

        ClearBackground(RAYWHITE);
        gui.draw();

        EndDrawing();
    }

    CloseWindow();

	printf("Closing editor..\n");

	return 0;
}
