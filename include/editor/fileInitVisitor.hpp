#ifndef _RPGPP_FILEINITVISITOR_H
#define _RPGPP_FILEINITVISITOR_H

#include <array>
#include <functional>

#include "services/fileSystemService.hpp"
#include "widgets/newFileDialog.hpp"

class FileInitVisitor {
private:
	std::array<std::function<void(NewFileDialog::Ptr)>, FILETYPE_MAX> funcs;

public:
	FileInitVisitor();
	void visit(EngineFileType fileType, NewFileDialog::Ptr dialog);
	bool funcIsEmpty(EngineFileType fileType);
	static void empty(NewFileDialog::Ptr dialog);
	static void tileset(NewFileDialog::Ptr dialog);
	static void room(NewFileDialog::Ptr dialog);
	static void actor(NewFileDialog::Ptr dialog);
	static void prop(NewFileDialog::Ptr dialog);
	static void dialogue(NewFileDialog::Ptr dialog);
	static void interactable(NewFileDialog::Ptr dialog);
	static void interface(NewFileDialog::Ptr dialog);
};

#endif
