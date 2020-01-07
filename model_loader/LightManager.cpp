#include "LightManager.h"

LightManager::LightManager() : _directional_count(0), _point_count(0), _spot_count(0), _directional_lights(),
_point_lights(), _spot_lights() 
{
}

void LightManager::render_ui()
{
    ImGui::Begin("Light Manager");
    ImGui::Text("Directional lights");
    ImGui::Separator();
    for (unsigned int i = 0; i < _directional_count; i++)
    {
        _directional_lights[i].draw_ui(i);
        ImGui::Separator();
    }

    if (ImGui::Button("Add directional"))
        add_directional(glm::vec3(0.0, -1.0, 0.0));

    ImGui::SameLine();
    if (ImGui::Button("Remove directional") && _directional_count > 0)
        remove_directional(_directional_count - 1);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Point lights");
    ImGui::Separator();
    for (unsigned int i = 0; i < _point_count; i++)
    {
        _point_lights[i].render_ui(i);
        ImGui::Separator();
    }

    if (ImGui::Button("Add point"))
        add_point(glm::vec3(0.0, 0.0, 0.0));

    ImGui::SameLine();
    if (ImGui::Button("Remove point") && _point_count > 0)
        remove_point(_point_count - 1);

    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Spacing();

    ImGui::Text("Spot lights");
    ImGui::Separator();
    for (unsigned int i = 0; i < _spot_count; i++)
    {
        _spot_lights[i].render_ui(i);
        ImGui::Separator();
    }

    if (ImGui::Button("Add spot"))
        add_spot(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), 12.5f, 15.0f);

    ImGui::SameLine();
    if (ImGui::Button("Remove spot") && _spot_count > 0)
        remove_spot(_spot_count - 1);

    ImGui::End();
}

void LightManager::add_directional(glm::vec3 direction, glm::vec3 ambient, 
    glm::vec3 diffuse, glm::vec3 specular)
{
    if (_directional_count == MAX_LIGHT)
    {
        Logger::add_entry(LogType::ERROR, "directional lights limit reach");
        return;
    }

    _directional_lights.push_back(DirectionalLight(_directional_count, direction, ambient, diffuse, specular));
    _directional_count++;
}

void LightManager::add_point(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, 
    glm::vec3 specular, float constant, float linear, float quadratic)
{
    if (_point_count == MAX_LIGHT)
    {
        Logger::add_entry(LogType::ERROR, "point lights limit reach");
        return;
    }

    _point_lights.push_back(PointLight(_point_count, position, ambient, diffuse, specular, constant, 
        linear, quadratic));
    _point_count++;
}

void LightManager::add_spot(glm::vec3 position, glm::vec3 direction, float cut_off,
    float outer_cur_off, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant, 
    float linear, float quadratic)
{
    if (_spot_count == MAX_LIGHT)
    {
        Logger::add_entry(LogType::ERROR, "spot lights limit reach");
        return;
    }

    _spot_lights.push_back(SpotLight(_spot_count, position, direction, cut_off, outer_cur_off, ambient,
        diffuse, specular, constant, linear, quadratic));
    _spot_count++;
}

void LightManager::remove_directional(unsigned int id)
{
    _directional_lights.erase(_directional_lights.begin() + id);
    _directional_count--;

    for (unsigned int i = id; i < _directional_count; i++)
        _directional_lights[i].set_name(i);
}

void LightManager::remove_point(unsigned int id)
{
    _point_lights.erase(_point_lights.begin() + id);
    _point_count--;

    for (unsigned int i = id; i < _point_count; i++)
        _point_lights[i].set_name(i);
}

void LightManager::remove_spot(unsigned int id)
{
    _spot_lights.erase(_spot_lights.begin() + id);
    _spot_count--;

    for (unsigned int i = id; i < _spot_count; i++)
        _point_lights[i].set_name(i);
}

void LightManager::set_uniforms(ShaderProgram& shader) const
{
    shader.set_int("directional_count", _directional_count);
    shader.set_int("point_count", _point_count);
    shader.set_int("spot_count", _spot_count);

    for (const auto& directional_light : _directional_lights)
        directional_light.set_uniforms(shader);
    
    for (const auto& point_light : _point_lights)
        point_light.set_uniforms(shader);

    for (const auto& spot_light : _spot_lights)
        spot_light.set_uniforms(shader);
}