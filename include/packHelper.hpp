#ifndef _RPGPP_PACK_HELPER
#define _RPGPP_PACK_HELPER

#include <cstdlib>
#include <cstring>

#include "rres.h"

// Load a continuous data buffer from rresResourceChunkData struct
inline unsigned char *LoadDataBuffer(rresResourceChunkData data, unsigned int rawSize) {
	unsigned char *buffer = (unsigned char *)calloc((data.propCount + 1) * sizeof(unsigned int) + rawSize, 1);

	memcpy(buffer, &data.propCount, sizeof(unsigned int));
	for (int i = 0; i < data.propCount; i++)
		memcpy(buffer + (i + 1) * sizeof(unsigned int), &data.props[i], sizeof(unsigned int));
	memcpy(buffer + (data.propCount + 1) * sizeof(unsigned int), data.raw, rawSize);

	return buffer;
}

// Unload data buffer
inline void UnloadDataBuffer(unsigned char *buffer) { free(buffer); }

#endif
