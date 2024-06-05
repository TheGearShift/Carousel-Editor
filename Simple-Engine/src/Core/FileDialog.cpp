#include "FileDialog.hpp"
#include <portable-file-dialogs.h>

void FileDialog::ShowWarningMessage(const std::string& title, const std::string& message) {
    pfd::message(
        title.c_str(),
        message.c_str(),
        pfd::choice::ok,
        pfd::icon::warning
    );
}

bool FileDialog::OpenFile(std::string& filePath) {
    const auto& selection = pfd::open_file(
        "Open",
        "",
        { "OBJ Wavefront Model (*.obj)", "*.obj", "All Files", "*" }
    ).result();

    if (!selection.empty()) {
        filePath = selection[0];
        return true;
    }

    return false;
}

bool FileDialog::SaveFile(std::string& filePath) {
    const auto& destination = pfd::save_file(
        "Save As",
        "",
        { "OBJ Wavefront Model (*.obj)", "*.obj" }
    ).result();

    if (!destination.empty()) {
        filePath = destination;
        size_t dotPosition = destination.find_last_of('.');

        if (dotPosition != std::string::npos && dotPosition < destination.length() - 1) {
            std::string extension = destination.substr(dotPosition, destination.length() - 1);

            if (extension == ".obj") {
                return true;
            }
        }

        filePath += ".obj";
        return true;
    }

    return false;
}