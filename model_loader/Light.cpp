#include "Light.h"

void Light::set_uniforms(ShaderProgram& shader)
{
    shader.set_vec3(_name + ".ambient", _ambient);
    shader.set_vec3(_name + ".diffuse", _diffuse);
    shader.set_vec3(_name + ".specular", _specular);
}