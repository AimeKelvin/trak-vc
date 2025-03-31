#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void commitChanges(const std::string& message) {
    if (!fs::exists(".trak/staging")) {
        std::cout << "Nothing to commit.\n";
        return;
    }

    std::string commitHash = std::to_string(time(0)); // Simple hash (timestamp-based)
    fs::create_directory(".trak/objects/" + commitHash);

    for (const auto& file : fs::directory_iterator(".trak/staging")) {
        fs::copy(file.path(), ".trak/objects/" + commitHash + "/" + file.path().filename());
    }

    std::ofstream log(".trak/log", std::ios::app);
    log << commitHash << " " << message << "\n";

    fs::remove_all(".trak/staging");

    std::cout << "Committed changes. Commit ID: " << commitHash << "\n";
}
