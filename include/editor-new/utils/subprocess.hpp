#ifndef _EDITOR_UTILS_SUBPROCESS_H
#define _EDITOR_UTILS_SUBPROCESS_H

#include <string>
#include <vector>

#include "edui/signal.hpp"
#include "subprocess.h"

namespace editor {
class Subprocess {
public:
	edui::SignalT<std::string> out;

	Subprocess();
	void update();

	int spawn(const char *commandLine[]);
	bool isRunning();

private:
	bool running = false;
	int procReturn;
	struct subprocess_s process;
};
}  // namespace editor

#endif
