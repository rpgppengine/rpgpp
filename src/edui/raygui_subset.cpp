#include "edui/raygui_subset.hpp"

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>

char **GuiLoadIconsFromMemory(const unsigned char *fileData, int dataSize, bool loadIconsName) {
	// Icon File Structure (.rgi)
	// ------------------------------------------------------
	// Offset  | Size    | Type       | Description
	// ------------------------------------------------------
	// 0       | 4       | char       | Signature: "rGI "
	// 4       | 2       | short      | Version: 100, 500
	// 6       | 2       | short      | reserved

	// 8       | 2       | short      | Num icons (N)
	// 10      | 2       | short      | Icons Size (Options: 16, 32, 64)

	// Icons name id (32 bytes per name id)
	// foreach (icon)
	// {
	//   12+32*i  | 32   | char       | Icon NameId
	// }

	// Icons data: One bit per pixel, stored as unsigned int array (depends on icon size)
	// Size*Size pixels/32bit per unsigned int = K unsigned int per icon
	// foreach (icon)
	// {
	//   ...   | K       | unsigned int | Icon Data
	// }
	// ------------------------------------------------------

	unsigned char *fileDataPtr = (unsigned char *)fileData;
	char **guiIconsName = NULL;

	char signature[5] = {0};
	short version = 0;
	short reserved = 0;
	short iconCount = 0;
	short iconSize = 0;

	memcpy(signature, fileDataPtr, 4);
	memcpy(&version, fileDataPtr + 4, sizeof(short));
	memcpy(&reserved, fileDataPtr + 4 + 2, sizeof(short));
	memcpy(&iconCount, fileDataPtr + 4 + 2 + 2, sizeof(short));
	memcpy(&iconSize, fileDataPtr + 4 + 2 + 2 + 2, sizeof(short));
	fileDataPtr += 12;

	if ((signature[0] == 'r') && (signature[1] == 'G') && (signature[2] == 'I') && (signature[3] == ' ')) {
		if (loadIconsName) {
			// NOTE: Always allocating RAYGUI_ICON_MAX_ICONS names slots
			guiIconsName = (char **)RAYGUI_CALLOC(RAYGUI_ICON_MAX_ICONS, sizeof(char *));
			for (int i = 0; i < iconCount; i++) {
				guiIconsName[i] = (char *)RAYGUI_CALLOC(RAYGUI_ICON_MAX_NAME_LENGTH, sizeof(char));
				memcpy(guiIconsName[i], fileDataPtr, RAYGUI_ICON_MAX_NAME_LENGTH);
				fileDataPtr += RAYGUI_ICON_MAX_NAME_LENGTH;
			}
		} else {
			// Skip icon name data if not required
			fileDataPtr += iconCount * RAYGUI_ICON_MAX_NAME_LENGTH;
		}

		int iconDataSize = iconCount * ((int)iconSize * (int)iconSize / 32) * (int)sizeof(unsigned int);
		guiIconsPtr = (unsigned int *)RAYGUI_CALLOC(iconDataSize, 1);

		memcpy(guiIconsPtr, fileDataPtr, iconDataSize);
	}

	return guiIconsName;
}

char **GuiLoadIcons(const char *fileName, bool loadIconsName) {
	FILE *rgiFile = fopen(fileName, "rb");
	char **guiIconsName = NULL;

	if (rgiFile != NULL) {
		unsigned char *fileData = NULL;
		int dataSize = 0;

		fseek(rgiFile, 0, SEEK_END);
		int size = (int)ftell(rgiFile);
		fseek(rgiFile, 0, SEEK_SET);

		if (size > 0) {
			fileData = (unsigned char *)RL_CALLOC(size, sizeof(unsigned char));
			// WARNING: File can be partially loaded but ignoring it for simplicity
			dataSize = (int)fread(fileData, sizeof(unsigned char), size, rgiFile);

			guiIconsName = GuiLoadIconsFromMemory(fileData, dataSize, loadIconsName);
		}

		fclose(rgiFile);
	}

	return guiIconsName;
}

void GuiDrawIcon(int iconId, int posX, int posY, int pixelSize, Color color) {
#define BIT_CHECK(a, b) ((a) & (1u << (b)))

	printf("%i \n", iconId);

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
