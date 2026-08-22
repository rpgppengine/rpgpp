#ifndef _EDUI_SIGNAL_H
#define _EDUI_SIGNAL_H

#include <functional>

namespace edui {
const short EDUI_MAX_CALLBACKS = 5;

struct Signal {
	std::array<std::function<void()>, EDUI_MAX_CALLBACKS> funcs = {};
	int size = 0;

	void connect(std::function<void()> func) {
		if (size >= EDUI_MAX_CALLBACKS) {
			return;
		}

		funcs[size] = func;
		size++;
	}

	void operator()(std::function<void()> newFunc) { connect(newFunc); }

	void disconnectAll() {
		for (auto &func : funcs) {
			func = nullptr;
		}
	}

	void invoke() {
		for (auto &func : funcs) {
			if (func != nullptr) {
				func();
			}
		}
	}
};

template <typename T>
struct SignalT {
	std::array<std::function<void(T)>, EDUI_MAX_CALLBACKS> funcs = {};
	int size = 0;

	void connect(std::function<void(T)> func) {
		if (size >= EDUI_MAX_CALLBACKS) {
			return;
		}

		funcs[size] = func;
		size++;
	}

	void operator()(std::function<void(T)> newFunc) { connect(newFunc); }

	void disconnectAll() {
		for (auto &func : funcs) {
			func = nullptr;
		}
	}

	void invoke(T a) {
		for (auto &func : funcs) {
			if (func != nullptr) {
				func(a);
			}
		}
	};
};

template <typename T, typename T2>
struct SignalT2 {
	std::array<std::function<void(T, T2)>, EDUI_MAX_CALLBACKS> funcs = {};
	int size = 0;

	void connect(std::function<void(T, T2)> func) {
		if (size >= EDUI_MAX_CALLBACKS) {
			return;
		}

		funcs[size] = func;
		size++;
	}

	void operator()(std::function<void(T, T2)> newFunc) { connect(newFunc); }

	void disconnectAll() {
		for (auto &func : funcs) {
			func = nullptr;
		}
	}

	void invoke(T a, T2 b) {
		for (auto &func : funcs) {
			if (func != nullptr) {
				func(a, b);
			}
		}
	};
};
}  // namespace edui

#endif
