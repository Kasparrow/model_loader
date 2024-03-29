#include "ModelManager.h"

ModelManager::ModelManager()
{

}

void ModelManager::initialize()
{
    scan_folders();

    assert(_models.size() > 0);
    _current_index = 0;
}

void ModelManager::scan_folders()
{
    _models.clear();

    for (const auto& path : fs_utils::model_directories)
    {
        Logger::add_entry(LogType::INFO, "scan " + path);
        int found = 0;
        fs_utils::scan_folder(path, fs_utils::model_extensions, true, [this, &found](const std::string& path) {
            _models.push_back(Model(path));
            found++;
        });
        Logger::add_entry(LogType::INFO, std::to_string(found) + " models found");

    }
}

void ModelManager::render(ShaderProgram& shader)
{
    _models[_current_index].render(shader);
}

void ModelManager::render_ui()
{
    ImGui::Begin("Model");

    ui_utils::combo_from_vector("Current", _models, _current_index, [](const auto& model) {
        return fs_utils::get_filename(model.get_path());
    });

    ImGui::Separator();

    _models[_current_index].render_ui();

    ImGui::End();
}