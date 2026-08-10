/*
 *
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

#ifndef _EDUI_RLICONS_H
#define _EDUI_RLICONS_H

#include <raylib.h>

#include "edui/rliconsarr.hpp"

#define RAYGUI_ICON_SIZE 16				  // Size of icons in pixels (squared)
#define RAYGUI_ICON_MAX_FONT_BACKED 257	  // Maximum number of icons to back in font atlas
#define RAYGUI_ICON_MAX_NAME_LENGTH 32	  // Maximum length of icon name id
#define RAYGUI_ICON_FONT_ATLAS_PADDING 1  // Padding between backed icons in font atlas

static float guiAlpha = 1.0f;			  // Gui controls transparency

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

void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color);

#endif
