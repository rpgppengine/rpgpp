#ifndef _EDITOR_SERIALIZE_INIARCHIVE_H
#define _EDITOR_SERIALIZE_INIARCHIVE_H

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <istream>
#include <stack>
#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "cereal/details/helpers.hpp"
#include "cereal/details/traits.hpp"
#include "cereal/macros.hpp"
#include "ini.h"
#include "raylib.h"
namespace editor {
class IniOutputArchive : public cereal::OutputArchive<IniOutputArchive>, public cereal::traits::TextArchive {
private:
	std::filesystem::path filePath;
	mINI::INIFile ini;
	mINI::INIStructure struc;
	mINI::INIMap<std::string> map;
	std::string section;
	std::stack<std::string> stack;
	std::stack<std::string> keys;
	std::stack<std::string> names;
	size_t fieldCount = 0;
	std::stack<size_t> fieldCounterStack;

	void write(const std::string &val) {
		std::string key = "val";
		if (names.empty())
			key = "value_" + std::to_string(fieldCount);
		else
			key = names.top();

		if (!section.empty()) {
			struc[section][key] = val;
		}

		fieldCount++;
	}

public:
	IniOutputArchive(std::filesystem::path path) : cereal::OutputArchive<IniOutputArchive>(this), ini(path) {
		this->filePath = path;
	}

	~IniOutputArchive() noexcept { bool res = ini.write(struc); }

	void startObj() {
		std::string res = "Object";

		fieldCounterStack.push(fieldCount);
		fieldCount = 0;

		if (!names.empty()) {
			res = names.top();
		} else {
			size_t parentIdx = fieldCounterStack.empty() ? 0 : fieldCounterStack.top();
			res = "section_" + std::to_string(parentIdx);
		}

		stack.push(section);
		if (section.empty()) {
			section = res;
		} else {
			section = section + "." + res;
		}
	}

	void endObj() {
		if (!stack.empty()) {
			section = stack.top();
			stack.pop();
		}
		if (!fieldCounterStack.empty()) {
			fieldCount = fieldCounterStack.top();
			fieldCounterStack.pop();
		}
		fieldCount++;
	}

	template <typename T>
	void saveValue(const cereal::NameValuePair<T> &pair) {
		names.push(pair.name);
		operator()(pair.value);
		names.pop();
	}

	void saveValue(const int &val) { write(std::to_string(val)); }
	void saveValue(const float &val) { write(std::to_string(val)); }
	void saveValue(const double &val) { write(std::to_string(val)); }
	void saveValue(const uint8_t &val) { write(std::to_string(val)); }
	void saveValue(const uint16_t &val) { write(std::to_string(val)); }
	void saveValue(const std::string &val) { write(val); }
};

class IniInputArchive : public cereal::InputArchive<IniInputArchive>, public cereal::traits::TextArchive {
private:
	std::filesystem::path filePath;
	mINI::INIFile ini;
	mINI::INIStructure struc;
	mINI::INIMap<std::string> map;

public:
	IniInputArchive(std::filesystem::path path) : cereal::InputArchive<IniInputArchive>(this), ini(path) {
		this->filePath = path;
		ini.read(struc);
	}

	template <typename T>
	void loadValue(T &val);

	template <int>
	void loadValue(int &val) {
		val = std::stoi(map["hi"]);
	}
};

template <class T>
inline typename std::enable_if<(cereal::traits::has_member_serialize<T, editor::IniOutputArchive>::value ||
								cereal::traits::has_member_save<T, editor::IniOutputArchive>::value)>::type
prologue(editor::IniOutputArchive &ar, T const &) {
	ar.startObj();
}

template <class T>
inline typename std::enable_if<(cereal::traits::has_member_serialize<T, editor::IniOutputArchive>::value ||
								cereal::traits::has_member_save<T, editor::IniOutputArchive>::value)>::type
epilogue(editor::IniOutputArchive &ar, T const &) {
	ar.endObj();
}

template <class T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = cereal::traits::sfinae>
inline void CEREAL_SAVE_FUNCTION_NAME(IniOutputArchive &out, T const &t) {
	out.saveValue(t);
}

template <typename T, cereal::traits::EnableIf<std::is_arithmetic<T>::value> = cereal::traits::sfinae>
inline void CEREAL_LOAD_FUNCTION_NAME(IniInputArchive &in, T &t) {
	in.loadValue(t);
}

template <class T>
inline void CEREAL_SAVE_FUNCTION_NAME(IniOutputArchive &out, cereal::NameValuePair<T> const &t) {
	out.saveValue(t);
}

template <typename T>
inline void CEREAL_LOAD_FUNCTION_NAME(IniInputArchive &in, cereal::NameValuePair<T> &t) {
	in.loadValue(t.value);
}

}  // namespace editor

CEREAL_REGISTER_ARCHIVE(editor::IniOutputArchive)
CEREAL_REGISTER_ARCHIVE(editor::IniInputArchive)
CEREAL_SETUP_ARCHIVE_TRAITS(editor::IniInputArchive, editor::IniOutputArchive)

#endif
