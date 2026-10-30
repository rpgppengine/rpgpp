#include "utils/subprocess.hpp"

#include "editor.hpp"
#include "subprocess.h"

using namespace editor;

Subprocess::Subprocess() {}

int Subprocess::spawn(const char *commandLine[]) {
	int result = subprocess_create(
		commandLine,
		subprocess_option_enable_async | subprocess_option_enable_async_no_wait | subprocess_option_inherit_environment,
		&process);

	if (result == 0) {
		running = true;
	} else {
		Editor::getInstance().getInterface().showErrorMessage("Spawning process failed.");
	}

	return result;
}

bool Subprocess::isRunning() { return running; }

void Subprocess::update() {
	if (running) {
		if (!subprocess_alive(&process)) {
			running = false;
			return;
		}

		char buf[4096] = {0};
		int res = subprocess_read_stdout(&process, buf, sizeof(buf));
		if (res > 0) {
			out.invoke(buf);
		}
	}
}
