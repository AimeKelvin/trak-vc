#include <iostream>
#include <filesystem>
#include <fstream>
#include <openssl/sha.h>

namespace fs = std::filesystem;

std::string computeFileHash(const std::string& filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file) return "";

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    
    char buffer[1024];
    while (file.read(buffer, sizeof(buffer))) {
        SHA256_Update(&ctx, buffer, file.gcount());
    }
    SHA256_Update(&ctx, buffer, file.gcount());

    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_Final(hash, &ctx);

    std::string hexHash;
    for (unsigned char c : hash) {
        hexHash += "0123456789abcdef"[c >> 4];
        hexHash += "0123456789abcdef"[c & 0xF];
    }

    return hexHash;
}

void addFileToTracking(const std::string& fileName) {
    if (!fs::exists(fileName)) {
        std::cout << "File not found: " << fileName << "\n";
        return;
    }

    std::string fileHash = computeFileHash(fileName);
    fs::create_directory(".trak/staging");

    std::ofstream outFile(".trak/staging/" + fileHash);
    std::ifstream inFile(fileName, std::ios::binary);
    outFile << inFile.rdbuf();

    std::cout << "Added " << fileName << " to staging area.\n";
}
