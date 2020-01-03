#pragma once
#ifndef _STR_UTILS_H_INCLUDED_
#define _STR_UTILS_H_INCLUDED_

#include <string>

namespace str_utils
{
    std::string erase_substring(const std::string& str, const std::string& to_erase);
    std::string get_file_extension(const std::string& filename);
}

#endif