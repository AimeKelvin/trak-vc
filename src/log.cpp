#include <iostream>
#include <fstream>

void showCommitHistory() {
    std::ifstream log(".trak/log");
    if (!log) {
        std::cout << "No commit history.\n";
        return;
    }

    std::string line;
    while (std::getline(log, line)) {
        std::cout << line << "\n";
    }
}
