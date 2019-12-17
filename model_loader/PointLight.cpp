#include "PointLight.h"

PointLight::PointLight(unsigned int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, 
    glm::vec3 specular, float constant, float linear, float quadratic) :
    Light(ambient, diffuse, specular), _position(position), _constant(constant), _linear(linear),
    _quadratic(quadratic) 
{
    set_name(id);
}

void PointLight::render_ui(unsigned int id)
{
    std::string counter = std::to_string(id);

    ImGui::InputFloat3(("PL Position " + counter).c_str(), &_position[0]);
    ImGui::InputFloat(("PL Constant " + counter).c_str(), &_constant);
    ImGui::InputFloat(("PL Linear " + counter).c_str(), &_linear);
    ImGui::InputFloat(("PL Quadratic " + counter).c_str(), &_quadratic);
    ImGui::ColorEdit3(("PL Ambient " + counter).c_str(), &_ambient[0]);
    ImGui::ColorEdit3(("PL Diffuse " + counter).c_str(), &_diffuse[0]);
    ImGui::ColorEdit3(("PL Specular " + counter).c_str(), &_specular[0]);
}

void PointLight::set_name(unsigned int id)
{
    _name = "point_lights[" + std::to_string(id) + "]" ;
}

void PointLight::set_uniforms(ShaderProgram& shader)
{
    Light::set_uniforms(shader);

    shader.set_vec3(_name + ".position", _position);
    shader.set_float(_name + ".constant", _constant);
    shader.set_float(_name + ".linear", _linear);
    shader.set_float(_name + ".quadratic", _quadratic);
}

void PointLight::render_dummy(ShaderProgram& shader, unsigned int vao)
{
    glm::mat4 light_model = glm::mat4(1.0f);
    light_model = glm::translate(light_model, _position);
    light_model = glm::scale(light_model, glm::vec3(0.2f));
    shader.set_mat4("model", light_model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}