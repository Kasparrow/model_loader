#ifndef _MODEL_H_INCLUDED_
#define _MODEL_H_INCLUDED_

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"

#include "Mesh.h"
#include "Utils.h"

class Model
{
public:
    Model(const std::string& path);
    void render(ShaderProgram& shader);
    glm::mat4 get_transformation_matrix() const;
    void render_ui();

    inline const std::string& get_path() const { return _path; }
    inline const glm::vec3& get_local_position() { return _local_position; };
    inline const glm::vec4& get_local_rotation() { return _local_rotation; };
    inline const float& get_local_scale() { return _local_scale; };

private:
    std::string _path;
    std::string _directory;
    std::vector<Mesh> _meshes;
    std::vector<Texture> _textures_loaded;
    float _local_scale;
    glm::vec4 _local_rotation;
    glm::vec3 _local_position;
    bool _is_loaded;
    bool _loading_failed;
    std::filesystem::file_time_type _last_write;

    void load_model(const std::string& path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(const aiScene* scene,aiMaterial* mat, aiTextureType type, std::string type_name);
    unsigned int load_texture_from_file(std::string filename, std::string directory);
    unsigned int load_texture_from_data(const aiTexture* texture);
    unsigned int load_texture(unsigned char* data, int width, int height, int nb_channels) const;
};

#endif