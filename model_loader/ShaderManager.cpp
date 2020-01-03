#include "ShaderManager.h"

ShaderManager::ShaderManager() : _current_index(0), _new_vertex(0), _new_fragment(0), _new_name("Default")
{

}

void ShaderManager::initialize()
{
    assert(std::filesystem::exists(DEFAULT_VERTEX) && std::filesystem::exists(DEFAULT_FRAGMENT));
    scan_shader_folder();
    add_shader(ShaderProgram(DEFAULT_NAME, DEFAULT_VERTEX, DEFAULT_FRAGMENT));
}

void ShaderManager::scan_shader_folder()
{
    _fragment_paths.clear();
    _vertex_paths.clear();

    for (const auto& entry : std::filesystem::directory_iterator(SHADER_FOLDER))
    {
        std::string path = entry.path().string();
        std::string filename = str_utils::erase_substring(path, SHADER_FOLDER);
        std::string extension = str_utils::get_file_extension(filename);

        if (extension == "frag")
            _fragment_paths.push_back(path);

        else if (extension == "vert")
            _vertex_paths.push_back(path);
    }
}

void ShaderManager::recompile_shaders()
{
    for (auto& shader : _shaders)
    {
        if (!std::filesystem::exists(shader.get_vertex_path()))
        {
            std::cout << "ERROR: Can't recompile " << shader.get_name() << " : " << shader.get_vertex_path() << " no longer exists.\n";
            continue;
        }

        if (!std::filesystem::exists(shader.get_fragment_path()))
        {
            std::cout << "ERROR: Can't recompile " << shader.get_name() << " : " << shader.get_fragment_path() << " no longer exists.\n";
            continue;
        }

        auto last_vertex_write = std::filesystem::last_write_time(shader.get_vertex_path());
        auto last_fragment_write = std::filesystem::last_write_time(shader.get_fragment_path());

        if (last_vertex_write > shader.get_last_vertex_write() || last_fragment_write > shader.get_last_fragment_write())
            shader.compile();
    }
}

void ShaderManager::add_shader(const ShaderProgram& shader)
{
    _shaders.push_back(shader);

    if (_shaders.size() == 1)
        _current_index = 0;
}

ShaderProgram& ShaderManager::get_current_shader()
{
    assert(_current_index >= 0 && _current_index <= _shaders.size() - 1);
    
    return _shaders[_current_index];
}

void ShaderManager::render()
{
    if (ImGui::BeginCombo("Current", _shaders[_current_index].get_name().c_str()))
    {
        for (unsigned int i = 0; i < _shaders.size(); i++)
        {
            bool is_selected = (i == _current_index);

            if (ImGui::Selectable(_shaders[i].get_name().c_str(), is_selected))
                _current_index = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }

    if (ImGui::Button("Recompile shaders"))
        recompile_shaders();

    ImGui::Separator();
    
    ImGui::InputText("Name", _new_name.data(), 20);

    if (ImGui::BeginCombo("Vertex", str_utils::erase_substring(_vertex_paths[_new_vertex], SHADER_FOLDER).c_str()))
    {
        for (unsigned int i = 0; i < _vertex_paths.size(); i++)
        {
            bool is_selected = (i == _new_vertex);

            if (ImGui::Selectable(str_utils::erase_substring(_vertex_paths[i], SHADER_FOLDER).c_str(), is_selected))
                _new_vertex = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }
    
    if (ImGui::BeginCombo("Fragment", str_utils::erase_substring(_fragment_paths[_new_fragment], SHADER_FOLDER).c_str()))
    {
        for (unsigned int i = 0; i < _fragment_paths.size(); i++)
        {
            bool is_selected = (i == _new_fragment);

            if (ImGui::Selectable(str_utils::erase_substring(_fragment_paths[i], SHADER_FOLDER).c_str(), is_selected))
                _new_fragment = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
    }
    
    auto it_name = std::find_if(_shaders.begin(), _shaders.end(), [this](ShaderProgram s) -> bool { 
        return s.get_name().compare(_new_name) == 0; 
    });

    if (ImGui::Button("Scan shader folder"))
        scan_shader_folder();

    ImGui::SameLine();

    if (ImGui::Button("Compile shader") && _new_name.size() > 0 && it_name == _shaders.end())
    {
        add_shader(ShaderProgram(_new_name, _vertex_paths[_new_vertex], _fragment_paths[_new_fragment]));
        _new_vertex = 0;
        _new_fragment = 0;
        _new_name = "Default";
    }
}
