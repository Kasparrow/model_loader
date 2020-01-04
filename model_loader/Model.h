#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image/stb_image.h"


#include "Mesh.h"

class Model
{
public:
    Model(std::string path);
    void render(ShaderProgram& shader);

    inline glm::vec3& get_local_position() { return _local_position; };
    inline glm::vec4& get_local_rotation() { return _local_rotation; };
    inline glm::vec3& get_local_scale() { return _local_scale; };

private:
    std::string _directory;
    std::vector<Mesh> _meshes;
    std::vector<Texture> _textures_loaded;
    glm::vec3 _local_scale;
    glm::vec4 _local_rotation;
    glm::vec3 _local_position;

    void load_model(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(const aiScene* scene,aiMaterial* mat, aiTextureType type, std::string type_name);
    unsigned int texture_from_file(std::string filename, std::string directory);
    unsigned int texture_from_data(const aiTexture* texture);
};

#endif