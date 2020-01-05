#ifndef _LIGHT_MANAGER_H_INCLUDED_
#define _LIGHT_MANAGER_H_INCLUDED_

#include <vector>

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#define MAX_LIGHT 20

class LightManager
{
public:
    LightManager();

    void set_uniforms(ShaderProgram& shader) const;
    void render_ui();
    
    void add_directional(glm::vec3 position, glm::vec3 ambient = DIRECTIONAL_AMBIENT,  
        glm::vec3 diffuse = DIRECTIONAL_DIFFUSE, glm::vec3 specular = DIRECTIONAL_SPECULAR);
    void add_point(glm::vec3 position, glm::vec3 ambient = POINT_AMBIENT,
        glm::vec3 diffuse = POINT_DIFFUSE, glm::vec3 specular = POINT_SPECULAR, 
        float constant = POINT_KC, float linear = POINT_KL, float quadratic = POINT_KQ);
    void add_spot(glm::vec3 position, glm::vec3 direction, float cut_off,
        float outer_cur_off, glm::vec3 ambient = SPOT_AMBIENT, glm::vec3 diffuse = SPOT_DIFFUSE, 
        glm::vec3 specular = SPOT_SPECULAR, float constant = SPOT_KC, float linear = SPOT_KL, 
        float quadratic = SPOT_KQ);

    void remove_directional(unsigned int id);
    void remove_point(unsigned int id);
    void remove_spot(unsigned int id);

    inline const std::vector<DirectionalLight>& get_directional_lights() const { return _directional_lights; }
    inline const std::vector<PointLight>& get_point_lights() const { return _point_lights; }
    inline const std::vector<SpotLight>& get_spot_lights() const { return _spot_lights; }
    inline const DirectionalLight& get_directional_light(unsigned int id) const { return _directional_lights[id]; }
    inline const PointLight& get_point_light(unsigned int id) const { return _point_lights[id]; }
    inline const SpotLight& get_spot_light(unsigned int id) const { return _spot_lights[id]; }
    inline int get_directional_count() const { return _directional_count; }
    inline int get_point_count() const { return _point_count; }
    inline int get_spot_count() const { return _spot_count; }

private:
    unsigned int _directional_count;
    unsigned int _point_count;
    unsigned int _spot_count;

    std::vector<DirectionalLight> _directional_lights;
    std::vector<PointLight> _point_lights;
    std::vector<SpotLight> _spot_lights;
};

#endif