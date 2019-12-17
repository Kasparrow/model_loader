#ifndef _DIRECTIONAL_LIGHT_H_INCLUDED_
#define _DIRECTIONAL_LIGHT_H_INCLUDED_

#include "Light.h"

#define DIRECTIONAL_AMBIENT glm::vec3(0.05f, 0.05f, 0.05f)
#define DIRECTIONAL_DIFFUSE glm::vec3(0.4f, 0.4f, 0.4f)
#define DIRECTIONAL_SPECULAR glm::vec3(0.5f, 0.5f, 0.5f)

class DirectionalLight : public Light
{
public:
    DirectionalLight(unsigned int id, glm::vec3 direction, glm::vec3 ambient,
        glm::vec3 diffuse, glm::vec3 specular);

    void draw_ui(unsigned int id);

    virtual void set_uniforms(ShaderProgram& shader);
    virtual void set_name(unsigned int id);

    inline glm::vec3& get_direction() { return _direction; }

private:
    glm::vec3 _direction;
};

#endif