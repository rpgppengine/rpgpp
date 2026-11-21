#ifndef _EDITOR_UTILS_VARIANTPROPERTYFACTORY_H
#define _EDITOR_UTILS_VARIANTPROPERTYFACTORY_H

#include <functional>
#include <map>
#include <memory>
#include <typeindex>
#include <variant>

#include "basicTypes.hpp"
#include "edui/container.hpp"
#include "edui/widget.hpp"
#include "widgets/values/valuewidget.hpp"
namespace editor {
struct VarFactoryData {
	std::function<std::shared_ptr<ValueWidget>()> instantiate;
};

class VariantPropertyFactory {
private:
	std::map<std::type_index, VarFactoryData> map;

	template <typename T, typename TWidget>
	void addEntry() {
		auto ti = std::type_index(typeid(T));
		VarFactoryData entry;
		entry.instantiate = [] {
			std::shared_ptr<TWidget> ptr = std::make_shared<TWidget>();
			return ptr;
		};
		map[ti] = entry;
	}

public:
	VariantPropertyFactory();

	template <typename T>
	VarFactoryData &getEntry() {
		auto ti = std::type_index(typeid(T));
		return map[ti];
	}

	VarFactoryData getEntryVar(ElementProperty &var) {
		return std::visit(
			[this, var](auto &&arg) {
				using T = std::decay_t<decltype(arg)>;
				auto ti = std::type_index(typeid(T));
				return map[ti];
			},
			var);
	}
};
}  // namespace editor

#endif
