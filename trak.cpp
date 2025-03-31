#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <ctime>

namespace fs = std::filesystem;

class Trak {
private:
    const std::string repoDir = ".trak";
    const std::string objectsDir = repoDir + "/objects";
    const std::string commitsDir = repoDir + "/commits";
    const std::string indexFile = repoDir + "/index";
    std::unordered_map<std::string, std::string> stagedFiles;

    std::string generateHash(const std::string& content) {
        std::hash<std::string> hasher;
        return std::to_string(hasher(content));
    }

    std::string readFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }

    void writeObject(const std::string& hash, const std::string& content) {
        std::string path = objectsDir + "/" + hash;
        std::ofstream file(path);
        file << content;
        file.close();
    }

    void loadIndex() {
        stagedFiles.clear();
        std::ifstream index(indexFile);
        if (!index.is_open()) return;
        std::string line;
        while (std::getline(index, line)) {
            size_t pos = line.find(':');
            if (pos != std::string::npos) {
                stagedFiles[line.substr(0, pos)] = line.substr(pos + 1);
            }
        }
        index.close();
    }

    void saveIndex() {
        std::ofstream index(indexFile, std::ios::trunc);
        for (const auto& [file, hash] : stagedFiles) {
            index << file << ":" << hash << "\n";
        }
        index.close();
    }

public:
    void init() {
        if (fs::exists(repoDir)) {
            std::cout << "Repository already exists in this directory.\n";
            return;
        }
        fs::create_directory(repoDir);
        fs::create_directory(objectsDir);
        fs::create_directory(commitsDir);
        std::ofstream index(indexFile);
        index.close();
        std::cout << "Initialized empty Trak repository in " << fs::current_path() << "/" << repoDir << "\n";
    }

    void add(const std::string& filepath) {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'init' first.\n";
            return;
        }
        if (!fs::exists(filepath)) {
            std::cout << "File '" << filepath << "' does not exist.\n";
            return;
        }
        loadIndex();
        std::string content = readFile(filepath);
        if (content.empty()) {
            std::cout << "File '" << filepath << "' is empty or unreadable.\n";
            return;
        }
        std::string hash = generateHash(content);
        stagedFiles[filepath] = hash;
        writeObject(hash, content);
        saveIndex();
        std::cout << "Added '" << filepath << "' to staging area.\n";
    }

    void commit(const std::string& message) {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'init' first.\n";
            return;
        }
        loadIndex();
        if (stagedFiles.empty()) {
            std::cout << "Nothing to commit.\n";
            return;
        }
        std::string commitContent;
        for (const auto& [file, hash] : stagedFiles) {
            commitContent += file + ":" + hash + "\n";
        }
        std::string commitHash = generateHash(commitContent);
        std::string commitPath = commitsDir + "/" + commitHash;
        std::ofstream commitFile(commitPath);
        commitFile << "Timestamp: " << std::time(nullptr) << "\n";
        commitFile << "Message: " << message << "\n";
        commitFile << commitContent;
        commitFile.close();
        stagedFiles.clear();
        saveIndex();
        std::cout << "Committed changes with hash: " << commitHash << "\n";
    }

    void log() {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'init' first.\n";
            return;
        }
        if (!fs::exists(commitsDir) || fs::is_empty(commitsDir)) {
            std::cout << "No commits yet.\n";
            return;
        }
        for (const auto& entry : fs::directory_iterator(commitsDir)) {
            std::ifstream commitFile(entry.path());
            std::string line;
            while (std::getline(commitFile, line)) {
                std::cout << line << "\n";
            }
            commitFile.close();
            std::cout << "----\n";
        }
    }

    void checkout(const std::string& commitHash) {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'init' first.\n";
            return;
        }
        std::string commitPath = commitsDir + "/" + commitHash;
        if (!fs::exists(commitPath)) {
            std::cout << "Commit '" << commitHash << "' not found.\n";
            return;
        }
        std::ifstream commitFile(commitPath);
        std::string line;
        while (std::getline(commitFile, line)) {
            if (line.find(':') != std::string::npos && line.find("Timestamp") == std::string::npos && line.find("Message") == std::string::npos) {
                size_t pos = line.find(':');
                std::string filepath = line.substr(0, pos);
                std::string hash = line.substr(pos + 1);
                std::string content = readFile(objectsDir + "/" + hash);
                std::ofstream outFile(filepath);
                outFile << content;
                outFile.close();
            }
        }
        commitFile.close();
        std::cout << "Checked out commit '" << commitHash << "'.\n";
    }
};

// Custom terminal function
void runTerminal() {
    Trak trak;
    std::string input;
    std::cout << "Welcome to Trak Terminal\n";
    std::cout << "Type 'help' for commands, 'exit' to quit.\n";

    while (true) {
        std::cout << "trak> ";
        std::getline(std::cin, input);
        if (input.empty()) continue;

        std::istringstream iss(input);
        std::vector<std::string> args;
        std::string arg;
        while (iss >> arg) {
            args.push_back(arg);
        }

        if (args[0] == "exit") {
            std::cout << "Goodbye!\n";
            break;
        } else if (args[0] == "help") {
            std::cout << "Commands:\n";
            std::cout << "  init              Initialize a new repository\n";
            std::cout << "  add <file>        Add a file to staging area\n";
            std::cout << "  commit -m <msg>   Commit staged files with a message\n";
            std::cout << "  log               Show commit history\n";
            std::cout << "  checkout <hash>   Restore files from a commit\n";
            std::cout << "  exit              Quit the terminal\n";
        } else if (args[0] == "init" && args.size() == 1) {
            trak.init();
        } else if (args[0] == "add" && args.size() == 2) {
            trak.add(args[1]);
        } else if (args[0] == "commit" && args.size() == 4 && args[1] == "-m") {
            trak.commit(args[2] + " " + args[3]); // Handle message with spaces
        } else if (args[0] == "log" && args.size() == 1) {
            trak.log();
        } else if (args[0] == "checkout" && args.size() == 2) {
            trak.checkout(args[1]);
        } else {
            std::cout << "Invalid command. Type 'help' for assistance.\n";
        }
    }
}

int main() {
    runTerminal();
    return 0;
}