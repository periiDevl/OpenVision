#include "ScriptsFunctions.h"


std::tuple<int, float, bool> Scripting::SetPosX(const std::string& filename) {
    std::ifstream infile(filename);

    // Read the last line from the file that contains "sto"
    std::string input;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find("stox") != std::string::npos) {
            input = line;
        }
    }

    infile.close();

    bool exists;

    // Return (-1, -1) if "sto" line is not found
    if (input.empty()) {
        exists = false;
        return std::make_tuple(0, 0, exists);
    }

    std::stringstream ss(input);
    std::string token;

    int first;
    float second;
    std::getline(ss, token, ':');
    std::getline(ss, token, ':');
    first = std::stoi(token);
    std::getline(ss, token);
    second = std::stoi(token);
    std::getline(ss, token);

    // Delete the contents of the file
    std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    exists = true;
    return std::make_tuple(first, second, exists);
}

std::tuple<int, float, bool> Scripting::SetPosY(const std::string& filename) {
    std::ifstream infile(filename);

    // Read the last line from the file that contains "sto"
    std::string input;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find("stoy") != std::string::npos) {
            input = line;
        }
    }

    infile.close();

    bool exists;

    // Return (-1, -1) if "sto" line is not found
    if (input.empty()) {
        exists = false;
        return std::make_tuple(0, 0, exists);
    }

    std::stringstream ss(input);
    std::string token;

    int first;
    float second;
    std::getline(ss, token, ':');
    std::getline(ss, token, ':');
    first = std::stoi(token);
    std::getline(ss, token);
    second = std::stoi(token);
    std::getline(ss, token);

    // Delete the contents of the file
    std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    exists = true;
    return std::make_tuple(first, second, exists);
}

