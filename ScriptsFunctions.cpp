#include "ScriptsFunctions.h"



std::tuple<int, float, bool> Scripting::SetFloat(const std::string& filename, std::string keyword) {
    std::ifstream infile(filename);

    std::string input;
    bool exists = false;

    // Iterate over each line in the file until we find the keyword we are looking for
    for (std::string line; std::getline(infile, line);) {
        if (line.find(keyword) != std::string::npos) {
            input = line;
            exists = true;
        }
    }

    infile.close();

    // Return (-1, -1) if the keyword line is not found
    if (!exists) {
        return std::make_tuple(0, 0, false);
    }

    // Extract the integer and float values from the keyword line
    int first;
    float second;
    std::stringstream ss(input);
    std::string token;
    std::getline(ss, token, ':');
    std::getline(ss, token, ':');
    first = std::stoi(token);
    std::getline(ss, token, ':');
    second = std::stof(token);

    // Clear the contents of the file
    std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
    outfile.close();

    return std::make_tuple(first, second, true);
}


std::tuple<int, std::string, bool> Scripting::SetString(const std::string& filename, std::string keyword) {
    std::ifstream infile(filename);

    std::string input, line;
    while (std::getline(infile, line)) {
        if (line.find(keyword) != std::string::npos) {
            input = line;
        }
    }

    infile.close();

    bool exists;
    if (input.empty()) {
        exists = false;
        return std::make_tuple(0, "", exists);
    }

    std::istringstream ss(input);
    std::string token;

    int first;
    std::string second;
    std::getline(ss, token, ':');
    std::getline(ss, token, ':');
    first = std::stoi(token);
    std::getline(ss, second);
    exists = true;

    std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
    outfile.close();

    return std::make_tuple(first, second, exists);
}


void Scripting::MicroLoadF(std::vector<Object>& sceneObjects, std::string filename, std::string keyword, float& value)
{
    auto [first, second, exists] = SetFloat(filename, keyword);

    if (exists) {
        value = second;
    }
}



void Scripting::MicroLoadLocationTex(std::vector<Object>& sceneObjects, std::string filename, std::string keyword, std::vector<Texture>& textures)
{
    std::string textureLocation;
    auto result = SetString(filename, keyword);
    first = std::get<0>(result);
    textureLocation = std::get<1>(result);
    exists = std::get<2>(result);

    if (exists) {
        for (size_t t = 0; t < textures.size(); t++)
        {
            if (textures[t].ImageFile == textureLocation)
            {
                sceneObjects[first].tex = textures[t];

            }
        }
    }
    

}

void Scripting::Load(std::string filename, std::vector<Object>& sceneObjects, std::vector<Texture>& textures)
{
    //Position
    //X
    MicroLoadF(sceneObjects, filename, "px", sceneObjects[first].calculatedPosition.x);

    MicroLoadF(sceneObjects, filename, "py", sceneObjects[first].calculatedPosition.y);
    MicroLoadF(sceneObjects, filename, "scax", sceneObjects[first].ScaleX);
    MicroLoadF(sceneObjects, filename, "scay", sceneObjects[first].ScaleY);
    MicroLoadF(sceneObjects, filename, "ang", sceneObjects[first].angle);
    MicroLoadF(sceneObjects, filename, "oulw", sceneObjects[first].outlineWidth);
    //Rgb
    MicroLoadF(sceneObjects, filename, "oulR", sceneObjects[first].OutlineColor.x);
    MicroLoadF(sceneObjects, filename, "oulG", sceneObjects[first].OutlineColor.y);
    MicroLoadF(sceneObjects, filename, "oulB", sceneObjects[first].OutlineColor.z);

    //Textures
    //MicroLoadF(sceneObjects, filename, "bt", );
    MicroLoadLocationTex(sceneObjects, filename, "bt_l",textures);


}


