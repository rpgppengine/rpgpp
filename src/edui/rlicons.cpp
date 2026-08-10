#include "edui/rlicons.hpp"

void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color) {
#define BIT_CHECK(a, b) ((a) & (1u << (b)))

	for (int i = 0, y = 0; i < RAYGUI_ICON_SIZE * RAYGUI_ICON_SIZE / 32; i++) {
		for (int k = 0; k < 32; k++) {
			if (BIT_CHECK(guiIcons[iconId * RAYGUI_ICON_DATA_ELEMENTS + i], k)) {
				GuiDrawRectangle(Rectangle{(float)posX + (k % RAYGUI_ICON_SIZE) * pixelSize,
										   (float)posY + y * pixelSize, (float)pixelSize, (float)pixelSize},
								 0, BLANK, color);
			}

			if ((k == 15) || (k == 31)) y++;
		}
	}
}
