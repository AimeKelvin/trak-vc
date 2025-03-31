#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

void initializeRepository() {
    if (fs::exists(".trak")) {
        std::cout << "Trak repository already initialized.\n";
        return;
    }

    fs::create_directory(".trak");
    fs::create_directory(".trak/objects");
    fs::create_directory(".trak/refs");

    std::ofstream config(".trak/config");
    config << "[trak]\nversion=1.0\n";
    config.close();

    std::cout << "Initialized empty Trak repository.\n";
}
