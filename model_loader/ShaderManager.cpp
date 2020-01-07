#include "ShaderManager.h"

ShaderManager::ShaderManager() : _current_index(0), _new_vertex(0), _new_fragment(0), _new_name("")
{

}

void ShaderManager::initialize()
{
    scan_folders();
    assert(_vertex_paths.size() > 0 && _fragment_paths.size() > 0);
    add_shader(ShaderProgram("Default", _vertex_paths[0], _fragment_paths[0]));
}

void ShaderManager::scan_folders()
{
    _vertex_paths.clear();
    _fragment_paths.clear();

    for (const auto& path : fs_utils::shader_directories)
    {
        Logger::add_entry(LogType::INFO, "scanning " + path);
        int vertex_found = 0;

        fs_utils::scan_folder(path, fs_utils::vertex_extensions, true, [this, &vertex_found](const std::string& path) {
            _vertex_paths.push_back(path);
            vertex_found++;
        });

        Logger::add_entry(LogType::INFO, "found " + std::to_string(vertex_found) + " vertex sources");

        int fragment_found = 0;
        fs_utils::scan_folder(path, fs_utils::fragment_extensions, true, [this, &fragment_found](const std::string& path) {
            _fragment_paths.push_back(path);
            fragment_found++;
        });

        Logger::add_entry(LogType::INFO, "found " + std::to_string(fragment_found) + " fragment sources");
    }
}

void ShaderManager::recompile_shaders()
{
    int recompiled_counter = 0;

    for (auto& shader : _shaders)
    {
        if (!std::filesystem::exists(shader.get_vertex_path()))
        {
            Logger::add_entry(LogType::ERROR, " can't recompile " + shader.get_name() + " : " + shader.get_vertex_path() + " no longer exists");
            continue;
        }

        if (!std::filesystem::exists(shader.get_fragment_path()))
        {
            Logger::add_entry(LogType::ERROR, " can't recompile " + shader.get_name() + " : " + shader.get_fragment_path() + " no longer exists");
            continue;
        }

        auto last_vertex_write = std::filesystem::last_write_time(shader.get_vertex_path());
        auto last_fragment_write = std::filesystem::last_write_time(shader.get_fragment_path());

        if (last_vertex_write > shader.get_last_vertex_write() || last_fragment_write > shader.get_last_fragment_write())
        {
            shader.compile();
            recompiled_counter++;
        }
    }

    if (recompiled_counter == 0)
        Logger::add_entry(LogType::INFO, "all shaders are up to date");

}

void ShaderManager::add_shader(const ShaderProgram& shader)
{
    _shaders.push_back(shader);

    if (_shaders.size() == 1)
        _current_index = 0;
}

void ShaderManager::render_ui()
{
    ImGui::Begin("Shader");

    ui_utils::combo_from_vector("Current", _shaders, _current_index, [](const auto& shader) {
        return shader.get_name();
    });

    if (ImGui::Button("Recompile shaders"))
        recompile_shaders();

    ImGui::Separator();
    ImGui::InputText("Name", &_new_name);
    
    ui_utils::combo_from_vector("Vertex", _vertex_paths, _new_vertex, [](const auto& path) {
        return fs_utils::get_filename(path);
    });

    ui_utils::combo_from_vector("Fragment", _fragment_paths, _new_fragment, [](const auto& path) {
        return fs_utils::get_filename(path);
    });

    if (ImGui::Button("Scan shader folder"))
        scan_folders();

    ImGui::SameLine();

    auto it_name = std::find_if(_shaders.begin(), _shaders.end(), [this](ShaderProgram s) -> bool {
        return s.get_name().compare(_new_name) == 0;
    });

    if (ImGui::Button("Compile shader"))
    {
        if (_new_name.size() == 0)
        {
            Logger::add_entry(LogType::ERROR, "shader must be named");
        }
        
        else if (it_name != _shaders.end())
        {
            Logger::add_entry(LogType::ERROR, "shader name must unique");
        }

        else
        {
            add_shader(ShaderProgram(_new_name, _vertex_paths[_new_vertex], _fragment_paths[_new_fragment]));
            _new_vertex = 0;
            _new_fragment = 0;
            _new_name = "";
        }
    }

    ImGui::End();
}
