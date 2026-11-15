#ifndef _EDITOR_UTILS_RESOURCEPACKER_H
#define _EDITOR_UTILS_RESOURCEPACKER_H

#include <filesystem>
#include <fstream>

#include "gamedata.hpp"

namespace editor {

class ResourcePacker {
public:
	ResourcePacker();
	void setBasePath(const std::filesystem::path &basePath);
	void setMinData(MinData data);
	void pack(const std::filesystem::path &destFile);

private:
	std::filesystem::path basePath;
	MinData minData;
	void packMinData(std::ofstream &out);
	void packResource(std::ofstream &out, const std::filesystem::path &filePath);

	void packImage(std::ofstream &out, const std::filesystem::path &filePath);
	void packText(std::ofstream &out, const std::filesystem::path &filePath);
	void packFont(std::ofstream &out, const std::filesystem::path &filePath);
	void packAudio(std::ofstream &out, const std::filesystem::path &filePath);
	void packWave(std::ofstream &out, const std::filesystem::path &filePath);
	void packMusic(std::ofstream &out, const std::filesystem::path &filePath);
	void packEngineResource(std::ofstream &out, const std::filesystem::path &filePath);
};
}  // namespace editor

#endif
