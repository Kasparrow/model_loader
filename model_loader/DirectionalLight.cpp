#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(unsigned int id, glm::vec3 direction, glm::vec3 ambient, 
    glm::vec3 diffuse, glm::vec3 specular) :
    Light(ambient, diffuse, specular), _direction(direction) 
{
    set_name(id);
}

void DirectionalLight::draw_ui(unsigned int id)
{
    std::string counter = std::to_string(id);

    ImGui::InputFloat3(("DL Direction " + counter).c_str(), &_direction[0]);
    ImGui::ColorEdit3(("DL Ambient " + counter).c_str(), &_ambient[0]);
    ImGui::ColorEdit3(("DL Diffuse " + counter).c_str(), &_diffuse[0]);
    ImGui::ColorEdit3(("DL Specular " + counter).c_str(), &_specular[0]);
}

void DirectionalLight::set_name(unsigned int id)
{
    _name = "dir_lights[" + std::to_string(id) + "]";
}

void DirectionalLight::set_uniforms(ShaderProgram& shader) const
{
    Light::set_uniforms(shader);
    shader.set_vec3(_name + ".direction", _direction);
}