#include "Model.h"

Model::Model(std::string path)
{
    load_model(path);
}

void Model::render(ShaderProgram& shader)
{
    for (unsigned int i = 0; i < _meshes.size(); i++)
        _meshes[i].render(shader);
}

void Model::load_model(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << "\n";
        return;
    }

    _directory = path.substr(0, path.find_last_of('\\'));

    process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _meshes.push_back(process_mesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        process_node(node->mChildren[i], scene);
    }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        vertex._position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex._normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

        if (mesh->mTextureCoords[0])
            vertex._tex_coords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        else
            vertex._tex_coords = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<Texture> diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture> specular_maps = load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString filename;
        material->GetTexture(type, i, &filename);
        bool skip = false;
        
        for (unsigned int j = 0; j < _textures_loaded.size(); j++)
        {
            if (std::strcmp(_textures_loaded[j]._path.data(), filename.C_Str()) == 0)
            {
                textures.push_back(_textures_loaded[j]);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture._id = texture_from_file(filename.C_Str(), _directory);
            texture._type = type_name;
            texture._path = filename.C_Str();
            textures.push_back(texture);
            _textures_loaded.push_back(texture);
        }
        
    }

    return textures;
}

unsigned int Model::texture_from_file(std::string filename, std::string directory)
{
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nb_channels;
    unsigned char* data = stbi_load((directory + '\\'+ filename).c_str(), &width, &height, &nb_channels, 0);

    if (data)
    {
        std::string extension = "";
        std::string::size_type index = filename.rfind(".");

        if (index != std::string::npos)
            extension = filename.substr(index + 1);

        if (extension == "png")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        else if (extension == "jpg")
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        else
        {
            std::cout << "Unknown extension : " << extension << "\n";
            stbi_image_free(data);
            return false;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    else
    {
        std::cout << "Failed to load texture : " << directory << "\\" << filename << ".\n";
        stbi_image_free(data);
        return false;
    }

    stbi_image_free(data);

    return id;
}