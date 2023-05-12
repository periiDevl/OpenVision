#ifndef SAVE_SYSTEM_CLASS_H
#define SAVE_SYSTEM_CLASS_H

#include <string>
#include <fstream>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sstream>
#include <iostream>

class SaveSystem
{
public:
    void save(const std::string& key, const std::string& value);
    void save(const std::string& key, const int&         value);
    void save(const std::string& key, const float&       value);
    void save(const std::string& key, const glm::vec2&   value);
    void save(const std::string& key, const glm::vec3&   value);

    void remove(const std::string& key);

    int getInt(const std::string& key, const int& default_value);
    float getFloat(const std::string& key, const float& default_value);
    std::string getString(const std::string& key, const std::string& default_value);
    glm::vec2 getVec2(const std::string& key, const glm::vec2& default_value) ;
    glm::vec3 getVec3(const std::string& key, const glm::vec3& default_value) ;

    void getString(const std::string& key);
    void getInt   (const std::string& key);
    void getFloat (const std::string& key);
    void getVec2  (const std::string& key);
    void getVec3  (const std::string& key);



    void load(const std::string& file_name);

    template <typename T>
    std::string getTypeString(const T& value);

    void saveToFile(const std::string& file_name);

private:
    std::unordered_map<std::string, std::string> data_;
};
#endif