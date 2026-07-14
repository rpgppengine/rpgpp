#include <raylib.h>

#include "game.hpp"

int main() {
	const int width = 640;
	const int height = 480;

	InitWindow(width, height, "Window");
	InitAudioDevice();

	Game game;
	game.init(true);

	game.useBin("game.bin");

	SetTargetFPS(60);

	while (!Game::windowWillClose()) {
		game.update();

		BeginDrawing();

		ClearBackground(RAYWHITE);

		game.draw();

		EndDrawing();
	}

	game.unload();

	CloseWindow();
	CloseAudioDevice();

	return 0;
}
