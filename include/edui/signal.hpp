#ifndef _EDUI_SIGNAL_H
#define _EDUI_SIGNAL_H

#include <functional>
namespace edui {
struct Signal {
	std::function<void()> func = nullptr;

	void invoke() {
		if (func != nullptr) {
			func();
		}
	}

	void operator =(std::function<void()> newFunc) {
		func = newFunc;
	}
};

template <typename T>
struct SignalT {
	std::function<void(T)> func = nullptr;

	void invoke() {
		if (func != nullptr) {
			func();
		}
	};

	void operator =(std::function<void(T)> newFunc) {
		func = newFunc;
	}
};

template <typename T, typename T2>
struct SignalT2 {
	std::function<void(T, T2)> func = nullptr;

	void invoke(T a, T2 b) {
		if (func != nullptr) {
			func(a, b);
		}
	};

	void operator =(std::function<void(T, T2)> newFunc) {
		func = newFunc;
	}
};
}

#endif
