#ifndef _LIGHT_H_INCLUDED_
#define _LIGHT_H_INCLUDED_

#include <string>
#include <glm/glm.hpp>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "ShaderProgram.h"

class Light
{
public:
    Light(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

    inline const glm::vec3& get_ambient() const { return _ambient; }
    inline const glm::vec3& get_diffuse() const { return _diffuse; }
    inline const glm::vec3& get_specular() const { return _specular; }
    
    inline void set_ambient(glm::vec3 ambient) { _ambient = ambient; }
    inline void set_diffuse(glm::vec3 diffuse) { _diffuse = diffuse; }
    inline void set_specular(glm::vec3 specular) { _specular = specular; }

    virtual void set_uniforms(ShaderProgram& shader) const;
    virtual void set_name(unsigned int id) = 0;

protected:
    std::string _name;
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
};

#endif