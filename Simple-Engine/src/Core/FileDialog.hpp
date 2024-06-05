#pragma once

#include <string>

class FileDialog {
public:
    static void ShowWarningMessage(const std::string& title, const std::string& message);
    static bool OpenFile(std::string& filePath);
    static bool SaveFile(std::string& filePath);
};