#pragma once
#include <iostream>
#include "Object.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <stack>
#include <variant>
#include <filesystem>

namespace fs = std::filesystem;

class FileFunction {
public:
    // Method to create a file
    void createFile(const char* filename) {
        std::ofstream file(filename);

        if (!file) {
            std::cerr << "Error creating file: " << filename << std::endl;
            return;
        }

        file.close();
    }

    // Method to copy a file
    void copyFile(const std::string& sourcePath, const std::string& destinationPath) {
        if (fs::exists(sourcePath)) {
            if (fs::exists(fs::path(destinationPath).parent_path())) {
                std::fstream source(sourcePath, std::ios::in | std::ios::binary);
                std::fstream destination(destinationPath, std::ios::out | std::ios::binary);

                char buffer[1024];
                while (source.read(buffer, sizeof(buffer))) {
                    destination.write(buffer, source.gcount());
                }

                source.close();
                destination.close();
            }
            else {
                std::cout << "Destination directory does not exist!" << std::endl;
            }
        }
        else {
            std::cout << "Source file does not exist!" << std::endl;
        }
    }

    // Method to create a folder
    void createFolder(const std::string& folderName) {
        if (fs::exists(folderName)) {
            std::filesystem::remove_all(folderName);
        }

        fs::create_directory(folderName);
    }

    // Method to check if a file exists
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
};
