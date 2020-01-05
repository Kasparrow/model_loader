#include "Light.h"

Light::Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    _ambient(ambient), _diffuse(diffuse), _specular(specular) 
{}

void Light::set_uniforms(ShaderProgram& shader) const
{
    shader.set_vec3(_name + ".ambient", _ambient);
    shader.set_vec3(_name + ".diffuse", _diffuse);
    shader.set_vec3(_name + ".specular", _specular);
}