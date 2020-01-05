#ifndef _POINT_LIGHT_H_INCLUDED_
#define _POINT_LIGHT_H_INCLUDED_

#include "Light.h"

#define POINT_AMBIENT glm::vec3(0.05f, 0.05f, 0.05f)
#define POINT_DIFFUSE glm::vec3(0.4f, 0.4f, 0.4f)
#define POINT_SPECULAR glm::vec3(0.5f, 0.5f, 0.5f)
#define POINT_KC 1.0f
#define POINT_KL 0.09f
#define POINT_KQ 0.032f

class PointLight : public Light
{
public:
    PointLight(unsigned int id, glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, 
        glm::vec3 specular, float constant, float linear, float quadratic);

    void render_ui(unsigned int id);

    virtual void set_uniforms(ShaderProgram& shader) const;
    virtual void set_name(unsigned int id);

    inline const glm::vec3& get_position() const { return _position; }

private:
    glm::vec3 _position;
    float _constant;
    float _linear;
    float _quadratic;
};

#endif