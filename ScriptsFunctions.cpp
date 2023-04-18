#include "ScriptsFunctions.h"
#include <future> // for std::async


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
    std::vector<std::future<void>> futures; // Vector to hold the futures of each async call

    //Position
    //X
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "px", std::ref(sceneObjects[first].calculatedPosition.x)));

    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "py", std::ref(sceneObjects[first].calculatedPosition.y)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "scax", std::ref(sceneObjects[first].ScaleX)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "scay", std::ref(sceneObjects[first].ScaleY)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "ang", std::ref(sceneObjects[first].angle)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "oulw", std::ref(sceneObjects[first].outlineWidth)));

    //Rgb
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "oulR", std::ref(sceneObjects[first].OutlineColor.x)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "oulG", std::ref(sceneObjects[first].OutlineColor.y)));
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadF, this, std::ref(sceneObjects), filename, "oulB", std::ref(sceneObjects[first].OutlineColor.z)));

    //Textures
    futures.push_back(std::async(std::launch::async, &Scripting::MicroLoadLocationTex, this, std::ref(sceneObjects), filename, "bt_l", std::ref(textures)));

    // Wait for all the futures to finish
    for (auto& f : futures) {
        f.get();
    }
}



