#pragma once
#ifndef _STR_UTILS_H_INCLUDED_
#define _STR_UTILS_H_INCLUDED_

#include <string>
#include <vector>
#include <functional>
#include <filesystem>

#include <iostream>

#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_stdlib.h>

#include "Logger.h"

namespace str_utils
{
    std::string erase_substring(const std::string& str, const std::string& to_erase);
    std::string vec3_to_string(glm::vec3 vec3);
}

namespace fs_utils
{
    const std::vector<std::string> model_extensions = { ".obj", ".fbx" };
    const std::vector<std::string> vertex_extensions = { ".vert", ".vs", ".vsh", ".vshader", ".vertex" };
    const std::vector<std::string> fragment_extensions = { ".frag", ".fs", ".fsh", ".fshader", ".fragment" };
    const std::vector<std::string> model_directories = { "..\\resources\\models", "..\\resources\\wow_models" };
    const std::vector<std::string> shader_directories = { "..\\resources\\shaders" };

    void scan_folder(const std::string& folder, const std::vector<std::string>& extensions, bool recursive, std::function<void(const std::string& filename)> callback);
    std::string get_file_extension(const std::string& path);
    std::string get_filename(const std::string& path);
    bool is_typeof_file(const std::string& filename, const std::vector<std::string>& extensions);
}

namespace ui_utils
{
    template <typename T, typename U, typename Func>
    void combo_from_vector(const std::string& label, const std::vector<T, U>& items, unsigned int& index, Func get_name)
    {
        // - lambda return string otherwise the pointer to the const char* is no longer valid
        if (ImGui::BeginCombo(label.c_str(), static_cast<std::string>(get_name(items[index])).c_str()))
        {
            for (unsigned int i = 0; i < items.size(); i++)
            {
                bool is_selected = (i == index);

                if (ImGui::Selectable(static_cast<std::string>(get_name(items[i])).c_str(), is_selected))
                    index = i;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        }
    }
}

#endif