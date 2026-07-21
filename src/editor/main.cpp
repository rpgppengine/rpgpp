#include "editor.hpp"
#include "edui/colorRect.hpp"
#include "edui/gui.hpp"
#include "edui/container.hpp"
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
	colorRect->layout.x = {0.5, 0};
	colorRect->layout.y = {0, 0};
	colorRect->layout.width = {0, 50};
	colorRect->layout.height = {0.5, 0};
	gui.add(colorRect);

	auto container = std::make_shared<edui::Container>();
	container->setPosition({0, 0}, {0, 5});
	container->setSize({0.5, 0}, {0.5, 0});
	container->render->padding = 4.0f;
	gui.add(container);

	auto newRect = std::make_shared<edui::ColorRect>();
	newRect->render->as<edui::ColorRectRender>().bgColor = MAGENTA;
	newRect->setPosition({0.5, 0}, {0, 0});
	newRect->setSize({0.25, 0}, {1, 0});
	container->add(newRect);

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
