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

private:
    std::string _directory;
    std::vector<Mesh> _meshes;
    std::vector<Texture> _textures_loaded;

    void load_model(std::string path);
    void process_node(aiNode* node, const aiScene* scene);
    Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, std::string type_name);
    unsigned int texture_from_file(std::string filename, std::string directory);
};

#endif