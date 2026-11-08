#ifndef _EDITOR_UTILS_RESOURCETYPEMANAGER_H
#define _EDITOR_UTILS_RESOURCETYPEMANAGER_H

#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "cereal/archives/json.hpp"
#include "edui/fileChooser.hpp"
#include "resource.hpp"
namespace editor {
struct ResourceFactory {
	std::function<std::unique_ptr<Resource>(std::string)> instantiate;
	std::function<void(std::filesystem::path, std::unique_ptr<Resource> &)> load;
	std::function<void(std::filesystem::path, std::unique_ptr<Resource> &)> save;
};
// std::unique_ptr<Resource>

class ResourceTypeManager {
public:
	ResourceTypeManager();
	const std::vector<edui::FileCategory> &getCategories();
	const edui::FileCategory &getCategory(const std::string &title);
	int getIconByExtension(const std::string &ext);
	const edui::FileCategory &getCategoryBYExtension(const std::string &ext);

	template <typename T>
	void addT(const std::string &title) {
		if (std::is_base_of_v<Resource, T>) {
			ResourceFactory data;
			data.instantiate = [](std::string) {
				std::unique_ptr<Resource> ptr = std::make_unique<T>();
				return std::move(ptr);
			};
			data.load = [](std::filesystem::path fpath, std::unique_ptr<Resource> &out) {
				printf("fpath: %s \n", fpath.c_str());
				out = std::make_unique<T>();
				out->loadFile(fpath);
			};
			data.save = [](std::filesystem::path fpath, std::unique_ptr<Resource> &ptr) {
				std::ofstream stream(fpath);

				T &res = ptr->as<T>();
				cereal::JSONOutputArchive ar(stream);
				ar(res);
			};
			factories[title] = data;
		}
	}

	bool hasFactory(const std::string &title);
	ResourceFactory &getFactory(const std::string &title);

private:
	std::vector<edui::FileCategory> types = {};
	std::map<std::string, ResourceFactory> factories = {};
	void addCategory(const std::string &title, std::vector<std::string> extensions, int iconId = ICON_FILE);
	void addDefaults();
};
}  // namespace editor

#endif
