#include "Utils.h"

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

    std::string get_file_extension(const std::string& filename)
    {
        std::string::size_type index = filename.rfind(".");

        // no extension
        if (index == std::string::npos)
            return "";

        return filename.substr(index + 1);
    }
}
