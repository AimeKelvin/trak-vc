#include <iostream>
#include "init.h"
#include "add.h"
#include "commit.h"
#include "log.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: trak <command>\n";
        return 1;
    }

    std::string command = argv[1];

    if (command == "init") {
        initializeRepository();
    } else if (command == "add" && argc == 3) {
        addFileToTracking(argv[2]);
    } else if (command == "commit" && argc == 4 && std::string(argv[2]) == "-m") {
        commitChanges(argv[3]);
    } else if (command == "log") {
        showCommitHistory();
    } else {
        std::cout << "Invalid command.\n";
    }

    return 0;
}
