#include "ScriptsFunctions.h"



std::tuple<int, float, bool> Scripting::SetFloat(const std::string& filename, std::string keyword) {
    std::ifstream infile(filename);

    // Read the last line from the file that contains "sto"
    std::string input;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find(keyword) != std::string::npos) {
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

std::tuple<int, std::string, bool> Scripting::SetString(const std::string& filename, std::string keyword) {
    std::ifstream infile(filename);

    // Read the last line from the file that contains "sto"
    std::string input;
    std::string line;
    while (std::getline(infile, line)) {
        if (line.find(keyword) != std::string::npos) {
            input = line;
        }

    }

    infile.close();

    bool exists;

    // Return (-1, -1) if "sto" line is not found
    if (input.empty()) {
        exists = false;
        return std::make_tuple(0, "", exists);
    }

    std::stringstream ss(input);
    std::string token;

    int first;
    std::string second;
    std::getline(ss, token, ':');
    std::getline(ss, token, ':');
    first = std::stoi(token);
    std::getline(ss, token);
    second = token;
    std::getline(ss, token);

    // Delete the contents of the file
    std::ofstream outfile(filename, std::ofstream::out | std::ofstream::trunc);
    outfile.close();
    exists = true;
    return std::make_tuple(first, second, exists);
}

void Scripting::MicroLoadF(std::vector<Object>& sceneObjects,std::string filename,std::string keyword, float& value)
{
    auto result = SetFloat(filename, keyword);

    first = std::get<0>(result);
    second = std::get<1>(result);
    exists = std::get<2>(result);

    if (sceneObjects.size() > 0 && exists)
    {
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


