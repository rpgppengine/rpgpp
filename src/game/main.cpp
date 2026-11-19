#include <raylib.h>

#include <system_error>

#include "game.hpp"
#include "sysDialogs.hpp"

int main() {
	const int width = 640;
	const int height = 480;

	InitWindow(width, height, "Window");
	InitAudioDevice();

	Game game;
	game.init();

	std::error_code ec;
	game.useBin("game.rres", ec);

	if (ec.value() != 0) {
		MessageBox("Error!", ec.message(), MessageType::OK, MessageIconType::ERROR, MessageDefaultButton::OK);
	}

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
