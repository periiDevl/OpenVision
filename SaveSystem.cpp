#include "SaveSystem.h"


void SaveSystem::load(const std::string& file_name)
{
    std::ifstream file(file_name);

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open file: " + file_name + " (File is already open using another program)");
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);

        std::string key;
        std::getline(iss, key, ':');

        std::string value_str;
        std::getline(iss, value_str, ':');

        std::string type_str;
        std::getline(iss, type_str, ':');

        if (type_str == "int")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else if (type_str == "float")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else if (type_str == "double")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else if (type_str == "string")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else if (type_str == "vec2")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else if (type_str == "vec3")
        {
            data_[key] = value_str + ":" + type_str;
        }
        else
        {
            throw std::runtime_error("Unsupported type: " + type_str);
        }
    }
}

void SaveSystem::save(const std::string& key, const std::string& value)
{
    std::stringstream ss;
    ss << value << ":" << "string";
    data_[key] = ss.str();
}
void SaveSystem::save(const std::string& key, const int& value)
{
    std::stringstream ss;
    ss << value << ":" << "int";
    data_[key] = ss.str();
}
void SaveSystem::save(const std::string& key, const float& value)
{
    std::stringstream ss;
    ss << value << ":" << "float";
    data_[key] = ss.str();
}
void SaveSystem::save(const std::string& key, const glm::vec2& value)
{
    std::stringstream ss;
    ss << value.x << "," << value.y << ":" << "vec2";
    data_[key] = ss.str();
}
void SaveSystem::save(const std::string& key, const glm::vec3& value)
{
    std::stringstream ss;
    ss << value.x << "," << value.y << "," << value.z << ":" << "vec3";
    data_[key] = ss.str();
}


void SaveSystem::remove(const std::string& key) {
    if (data_.find(key) == data_.end()) {
        return;
    }
    data_.erase(key);
}

int SaveSystem::getInt(const std::string& key, const int& default_value) 
{
    if (data_.find(key) == data_.end())
    {
        save(key, default_value);
        return default_value;
    }

    std::string value_str = data_.at(key);
    int separator_pos = value_str.find_last_of(':');
    std::string type_str = value_str.substr(separator_pos + 1);
    value_str = value_str.substr(0, separator_pos);

    return std::stoi(value_str);
}
float SaveSystem::getFloat(const std::string& key, const float& default_value) 
{
    if (data_.find(key) == data_.end())
    {
        save(key, default_value);
        return default_value;
    }

    std::string value_str = data_.at(key);
    int separator_pos = value_str.find_last_of(':');
    std::string type_str = value_str.substr(separator_pos + 1);
    value_str = value_str.substr(0, separator_pos);

    return std::stof(value_str);
}
std::string SaveSystem::getString(const std::string& key, const std::string& default_value)
{
    if (data_.find(key) == data_.end())
    {
        save(key, default_value);
        return default_value;
    }

    return data_.at(key).substr(0, data_.at(key).find_last_of(':'));
}
glm::vec2 SaveSystem::getVec2(const std::string& key, const glm::vec2& default_value)
{
    if (data_.find(key) == data_.end())
    {
        save(key, default_value);
        return default_value;
    }

    std::string value_str = data_.at(key);
    int separator_pos = value_str.find_last_of(':');
    std::string type_str = value_str.substr(separator_pos + 1);
    value_str = value_str.substr(0, separator_pos);

    glm::vec2 result;
    int comma_pos = value_str.find(',');
    result.x = std::stof(value_str.substr(0, comma_pos));
    result.y = std::stof(value_str.substr(comma_pos + 1));
    return result;
}
glm::vec3 SaveSystem::getVec3(const std::string& key, const glm::vec3& default_value)
{
    if (data_.find(key) == data_.end())
    {
        save(key, default_value);
        return default_value;
    }

    std::string value_str = data_.at(key);
    int separator_pos = value_str.find_last_of(':');
    std::string type_str = value_str.substr(separator_pos + 1);
    value_str = value_str.substr(0, separator_pos);

    glm::vec3 result;
    int comma_pos = value_str.find(',');
    result.x = std::stof(value_str.substr(0, comma_pos));
    int second_comma_pos = value_str.find(',', comma_pos + 1);
    result.y = std::stof(value_str.substr(comma_pos + 1, second_comma_pos - comma_pos - 1));
    result.z = std::stof(value_str.substr(second_comma_pos + 1));
    return result;
}

void SaveSystem::saveToFile(const std::string& file_name)
{
    std::ofstream file(file_name);

    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing: " << file_name << std::endl;
        return;
    }

    for (const auto& [key, value] : data_)
    {
        // Write key, value, and type information to file
        file << key << ":" << value << std::endl;
    }

    file.close();
}
