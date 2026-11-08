#ifndef _RESOURCE_H
#define _RESOURCE_H

#include <filesystem>
class Resource {
public:
	virtual void loadFile(std::filesystem::path filePath) = 0;
	virtual void saveFile(std::filesystem::path filePath) = 0;

	template <typename T>
	T &as() {
		return static_cast<T &>(*this);
	}
};

#endif
