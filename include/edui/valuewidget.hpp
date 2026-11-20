#ifndef _EDUI_VALUEWIDGET_H
#define _EDUI_VALUEWIDGET_H

#include <variant>

#include "basicTypes.hpp"
#include "edui/signal.hpp"
#include "edui/widget.hpp"

namespace edui {
struct ValueWidget {
	// virtual void setValue(const ElementProperty &val) = 0;
	// virtual ElementProperty getValueV() = 0;
};

template <typename T>
struct ValueWidgetT : public ValueWidget {
	SignalT2<T, T> onValueChanged;

	virtual void setValue(const T &val) = 0;
	virtual T getValue() = 0;

	/*
	void setValue(const ElementProperty &val) override {
		if (std::holds_alternative<T>(val)) {
			setValue(std::get<T>(val));
		}
	}


	ElementProperty getValueV() override { return getValue(); }*/
};
}  // namespace edui

#endif
