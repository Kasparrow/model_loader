#ifndef _SPOT_LIGHT_H_INCLUDED_
#define _SPOT_LIGHT_H_INCLUDED_

#include "Light.h"

#define SPOT_AMBIENT glm::vec3(0.05f, 0.05f, 0.05f)
#define SPOT_DIFFUSE glm::vec3(0.4f, 0.4f, 0.4f)
#define SPOT_SPECULAR glm::vec3(0.5f, 0.5f, 0.5f)
#define SPOT_KC 1.0f
#define SPOT_KL 0.09f
#define SPOT_KQ 0.032f


class SpotLight : public Light
{
public:
    SpotLight(unsigned int id, glm::vec3 position, glm::vec3 direction, float cut_off,
        float outer_cut_off, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float constant,
        float linear, float quadratic);

    void render_ui(unsigned int id);

    virtual void set_uniforms(ShaderProgram& shader);
    virtual void set_name(unsigned int id);
    
    inline void set_position(glm::vec3 position) { _position = position;  }
    inline void set_direction(glm::vec3 direction) { _direction = direction; }

private:
    glm::vec3 _position;
    glm::vec3 _direction;
    float _constant;
    float _linear;
    float _quadratic;
    float _cut_off;
    float _outer_cut_off;
};

#endif