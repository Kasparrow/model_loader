#include "Utils.h"

#include <iostream>

namespace str_utils
{
    std::string erase_substring(const std::string& str, const std::string& to_erase)
    {
        std::string res = str;

        size_t index = res.find(to_erase);

        if (index != std::string::npos)
            return res.erase(index, to_erase.length());

        return res;
    }

    std::string vec3_to_string(glm::vec3 vec3)
    {
        return "(" + std::to_string(vec3.x) + ", " + std::to_string(vec3.y) + ", " + std::to_string(vec3.z) + ")";
    }
}

namespace fs_utils
{
    void scan_folder(const std::string& folder, const std::vector<std::string>& extensions, bool recursive, std::function<void(const std::string& filename)> callback)
    {
        if (!std::filesystem::exists(folder))
            return;

        for (const auto& entry : std::filesystem::directory_iterator(folder))
        {
            std::string path = entry.path().string();
            
            if (is_typeof_file(path, extensions))
                callback(path);

            else if (entry.is_directory() && recursive)
                scan_folder(path, extensions, recursive, callback);
        }
    }

    std::string get_file_extension(const std::string& path)
    {
        std::string::size_type index = path.rfind(".");

        if (index == std::string::npos)
            return "";

        return path.substr(index);
    }

    std::string get_filename(const std::string& path)
    {
        std::string::size_type index = path.rfind("\\");

        if (index == std::string::npos)
            return std::string(path);

        return path.substr(index + 1);
    }

    bool is_typeof_file(const std::string& filename, const std::vector<std::string>& extensions)
    {
        std::string file_extension = get_file_extension(filename);
        
        return std::find_if(extensions.begin(), extensions.end(), [file_extension](const std::string& extension) {
            return file_extension.compare(extension) == 0;
        }) != extensions.end();
    }
}