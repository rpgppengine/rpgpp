/*
 * This is only a subset of the raygui library header file, just enough to load and draw icons.
 * Original project: https://github.com/raysan5/raygui
 */
/*******************************************************************************************
 *   LICENSE: zlib/libpng
 *
 *   Copyright (c) 2014-2026 Ramon Santamaria (@raysan5)
 *
 *   This software is provided "as-is", without any express or implied warranty. In no event
 *   will the authors be held liable for any damages arising from the use of this software.
 *
 *   Permission is granted to anyone to use this software for any purpose, including commercial
 *   applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *     1. The origin of this software must not be misrepresented; you must not claim that you
 *     wrote the original software. If you use this software in a product, an acknowledgment
 *     in the product documentation would be appreciated but is not required.
 *
 *     2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *     as being the original software.
 *
 *     3. This notice may not be removed or altered from any source distribution.
 *
 **********************************************************************************************/

#ifndef _RAYGUI_SUBSET_H
#define _RAYGUI_SUBSET_H

#include <raylib.h>

#ifndef RAYGUIAPI
#define RAYGUIAPI  // Functions defined as 'extern' by default (implicit specifiers)
#endif

#define RAYGUI_MALLOC(sz) malloc(sz)
#define RAYGUI_CALLOC(n, sz) calloc(n, sz)
#define RAYGUI_FREE(p) free(p)

#include "editor-new/icons.hpp"

// Load icons from memory
// GLOBAL: Updates global variable: guiIconsPtr
char **GuiLoadIconsFromMemory(const unsigned char *fileData, int dataSize, bool loadIconsName);

// Load raygui icons file (.rgi)
// WARNING: guiIconsName[]][] memory should be manually freed!
char **GuiLoadIcons(const char *fileName, bool loadIconsName);

void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color);

static float guiAlpha = 1.0f;  // Gui controls transparency

static void GuiDrawRectangle(Rectangle rec, int borderWidth, Color borderColor, Color color) {
	if (color.a > 0) {
		// Draw rectangle filled with color
		DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, (int)rec.height, Fade(color, guiAlpha));
	}

	if (borderWidth > 0) {
		// Draw rectangle border lines with color
		DrawRectangle((int)rec.x, (int)rec.y, (int)rec.width, borderWidth, Fade(borderColor, guiAlpha));
		DrawRectangle((int)rec.x, (int)rec.y + borderWidth, borderWidth, (int)rec.height - 2 * borderWidth,
					  Fade(borderColor, guiAlpha));
		DrawRectangle((int)rec.x + (int)rec.width - borderWidth, (int)rec.y + borderWidth, borderWidth,
					  (int)rec.height - 2 * borderWidth, Fade(borderColor, guiAlpha));
		DrawRectangle((int)rec.x, (int)rec.y + (int)rec.height - borderWidth, (int)rec.width, borderWidth,
					  Fade(borderColor, guiAlpha));
	}
}

#endif
