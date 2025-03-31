#include <iostream>
#include <fstream>
#include <sstream>
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
    std::unordered_map<std::string, std::string> stagedFiles; // filename -> hash

    // Generate a simple hash (simulating SHA-1)
    std::string generateHash(const std::string& content) {
        std::hash<std::string> hasher;
        return std::to_string(hasher(content));
    }

    // Read file content
    std::string readFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) return "";
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        return buffer.str();
    }

    // Write content to objects directory
    void writeObject(const std::string& hash, const std::string& content) {
        std::string path = objectsDir + "/" + hash;
        std::ofstream file(path);
        file << content;
        file.close();
    }

    // Load staged files from index
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

    // Save staged files to index
    void saveIndex() {
        std::ofstream index(indexFile, std::ios::trunc);
        for (const auto& [file, hash] : stagedFiles) {
            index << file << ":" << hash << "\n";
        }
        index.close();
    }

public:
    // Initialize repository
    void init() {
        if (fs::exists(repoDir)) {
            std::cout << "Repository already exists in this directory.\n";
            return;
        }
        fs::create_directory(repoDir);
        fs::create_directory(objectsDir);
        fs::create_directory(commitsDir);
        std::ofstream index(indexFile); // Create empty index
        index.close();
        std::cout << "Initialized empty Trak repository in " << fs::current_path() << "/" << repoDir << "\n";
    }

    // Add file to staging area
    void add(const std::string& filepath) {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'trak init' first.\n";
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

    // Commit staged files
    void commit(const std::string& message) {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'trak init' first.\n";
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

    // Show commit history
    void log() {
        if (!fs::exists(repoDir)) {
            std::cout << "No Trak repository found. Run 'trak init' first.\n";
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
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: trak <command> [args]\n";
        std::cout << "Commands:\n";
        std::cout << "  init              Initialize a new repository\n";
        std::cout << "  add <file>        Add a file to staging area\n";
        std::cout << "  commit -m <msg>   Commit staged files with a message\n";
        std::cout << "  log               Show commit history\n";
        return 1;
    }

    Trak trak;
    std::string command = argv[1];

    if (command == "init") {
        trak.init();
    } else if (command == "add" && argc == 3) {
        trak.add(argv[2]);
    } else if (command == "commit" && argc == 4 && std::string(argv[2]) == "-m") {
        trak.commit(argv[3]);
    } else if (command == "log") {
        trak.log();
    } else {
        std::cout << "Invalid command or arguments. Use 'trak' for help.\n";
    }

    return 0;
}