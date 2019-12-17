#ifndef _MESH_H_INCLUDED_
#define _MESH_H_INCLUDED_

#include <string>
#include <vector>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "ShaderProgram.h"

struct Vertex
{
    glm::vec3 _position;
    glm::vec3 _normal;
    glm::vec2 _tex_coords;
};

struct Texture
{
    unsigned int _id;
    std::string _type;
    std::string _path;
};

class Mesh
{
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> _indices, std::vector<Texture> textures);
    void render(ShaderProgram& shader);

private:

    unsigned int _vao;
    unsigned int _vbo;
    unsigned int _ebo;

    std::vector<Vertex> _vertices;
    std::vector<unsigned int> _indices;
    std::vector<Texture> _textures;

    void initialize();
};

#endif