#include "SpotLight.h"

SpotLight::SpotLight(unsigned int id, glm::vec3 position, glm::vec3 direction, float cut_off,
    float outer_cut_off, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
    float linear, float quadratic) : Light(ambient, diffuse, specular), _position(position),
    _direction(direction), _constant(constant), _linear(linear), _quadratic(quadratic),
    _cut_off(glm::cos(glm::radians(cut_off))), _outer_cut_off(glm::cos(glm::radians(outer_cut_off))) 
{
    set_name(id);
}

void SpotLight::render_ui(unsigned int id)
{
    std::string counter = std::to_string(id);

    ImGui::InputFloat3(("SL Position " + counter).c_str(), &_position[0]);
    ImGui::InputFloat3(("SL Direction " + counter).c_str(), &_direction[0]);
    ImGui::InputFloat(("SL Constant " + counter).c_str(), &_constant);
    ImGui::InputFloat(("SL Linear " + counter).c_str(), &_linear);
    ImGui::InputFloat(("SL Quadratic " + counter).c_str(), &_quadratic);
    ImGui::ColorEdit3(("SL Ambient " + counter).c_str(), &_ambient[0]);
    ImGui::ColorEdit3(("SL Diffuse " + counter).c_str(), &_diffuse[0]);
    ImGui::ColorEdit3(("SL Specular " + counter).c_str(), &_specular[0]);
}

void SpotLight::set_name(unsigned int id)
{
    _name = "spot_lights[" + std::to_string(id) + "]";
}

void SpotLight::set_uniforms(ShaderProgram& shader)
{
    Light::set_uniforms(shader);
    shader.set_float(_name + ".constant", _constant);
    shader.set_float(_name + ".linear", _linear);
    shader.set_float(_name + ".quadratic", _quadratic);
    shader.set_vec3(_name + ".position", _position);
    shader.set_vec3(_name + ".direction", _direction);
    shader.set_float(_name + ".cut_off",  _cut_off);
    shader.set_float(_name + ".outer_cut_off", _outer_cut_off);
}