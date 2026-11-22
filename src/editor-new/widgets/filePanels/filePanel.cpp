#include "widgets/filePanels/filePanel.hpp"

#include <filesystem>
#include <utility>

#include "edui/container.hpp"

using namespace editor;

FilePanel::FilePanel() : edui::Container() { render->border = 0; }

void FilePanel::setResource(std::unique_ptr<Resource> ptr) { resource.swap(ptr); }

void FilePanel::setSourcePath(const std::filesystem::path &path) { this->sourcePath = path; }

std::filesystem::path FilePanel::getSourcePath() { return sourcePath; }
