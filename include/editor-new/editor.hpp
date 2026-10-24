#ifndef _EDITOR_EDITOR_H
#define _EDITOR_EDITOR_H

#include <memory>

#include "services/interface.hpp"
#include "services/resources.hpp"

namespace editor {
const int EDITOR_WINDOW_WIDTH = 800;
const int EDITOR_WINDOW_HEIGHT = 600;
const int EDITOR_FPS = 60;

class Editor {
public:
	Editor();
	~Editor();

	void run();

	void update();
	void draw();
	void unload();

	static Editor &getInstance();

	Resources &getResources();
	Interface &getInterface();

private:
	static Editor *instance;

	std::unique_ptr<Resources> resources = nullptr;
	std::unique_ptr<Interface> interface = nullptr;
	void initWindow();
};
}  // namespace editor

#endif
