#ifndef _EDUI_VALUEWIDGET_H
#define _EDUI_VALUEWIDGET_H

#include "edui/signal.hpp"
#include "edui/widget.hpp"

namespace edui {
struct ValueWidget {};

template <typename T>
struct ValueWidgetT : public ValueWidget {
	SignalT2<T, T> onValueChangedT;

	virtual void setValue(const T& val) = 0;
	virtual T getValue() = 0;
};
}  // namespace edui

#endif
