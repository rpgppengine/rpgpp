#ifndef _EDITOR_UTILS_RESOURCETYPEMANAGER_H
#define _EDITOR_UTILS_RESOURCETYPEMANAGER_H

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "cereal/archives/json.hpp"
#include "cereal/archives/portable_binary.hpp"
#include "edui/fileChooser.hpp"
#include "resource.hpp"

namespace editor {
enum ResourceType {
	RESOURCE_TILESET,
	RESOURCE_ROOM,
	RESOURCE_ACTOR,
	RESOURCE_PROP,
	RESOURCE_DIALOUGUE,
	RESOURCE_UI,
	RESOURCE_INVALID
};

struct ResourceFactory {
	RPGPP_RESOURCE typeId;
	std::function<std::unique_ptr<Resource>(std::string)> instantiate;
	std::function<void(std::filesystem::path, std::unique_ptr<Resource> &)> load;
	std::function<void(std::filesystem::path, std::unique_ptr<Resource> &)> save;
	std::function<std::string(std::filesystem::path)> saveBin;
};

class ResourceTypeManager {
public:
	ResourceTypeManager();
	const std::vector<edui::FileCategory> &getCategories();
	const edui::FileCategory &getCategory(const std::string &title);
	int getIconByExtension(const std::string &ext);
	const edui::FileCategory &getCategoryBYExtension(const std::string &ext);
	bool getTypeTitleByExtension(const std::string &ext, std::string *title);
	size_t totalResourceCount(const std::filesystem::path &base);

	template <typename T>
	void addT(const std::string &title, RPGPP_RESOURCE typeId) {
		if (std::is_base_of_v<Resource, T>) {
			enums.push_back(title);

			ResourceFactory data;
			data.typeId = typeId;
			data.instantiate = [](std::string) {
				std::unique_ptr<Resource> ptr = std::make_unique<T>();
				return std::move(ptr);
			};
			data.load = [](std::filesystem::path fpath, std::unique_ptr<Resource> &out) {
				printf("fpath: %s \n", fpath.c_str());
				/*
				out = std::make_unique<T>();
				out->loadFile(fpath);
				*/
				std::ifstream stream(fpath);

				out = std::make_unique<T>();
				T &res = out->as<T>();
				cereal::JSONInputArchive ar(stream);
				ar(res);
			};
			data.save = [](std::filesystem::path fpath, std::unique_ptr<Resource> &ptr) {
				std::ofstream stream(fpath);

				ptr->beforeSave();
				T &res = ptr->as<T>();
				cereal::JSONOutputArchive ar(stream);
				ar(res);
			};
			data.saveBin = [](std::filesystem::path fpath) {
				std::shared_ptr<Resource> resource = std::make_shared<T>();
				resource->beforeSave();

				std::ifstream stream(fpath);
				T &res = resource->as<T>();
				cereal::JSONInputArchive ar(stream);
				ar(res);

				std::stringstream str;
				cereal::PortableBinaryOutputArchive binAr(str);
				binAr(res);
				std::string final = str.str();
				return final;
			};
			factories[title] = data;
		}
	}

	bool hasFactory(const std::string &title);
	ResourceFactory &getFactory(const std::string &title);
	RPGPP_RESOURCE getEnum(const std::string &title);
	std::vector<std::filesystem::path> findAllOfType(const std::filesystem::path &base, const std::string &title);

private:
	std::vector<std::string> enums = {};
	std::vector<edui::FileCategory> types = {};
	std::map<std::string, ResourceFactory> factories = {};
	void addCategory(const std::string &title, std::vector<std::string> extensions, int iconId = ICON_FILE);
	void addDefaults();
};
}  // namespace editor

#endif
