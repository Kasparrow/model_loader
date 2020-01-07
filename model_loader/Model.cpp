#include "Model.h"

Model::Model(const std::string& path) : _path(path), _is_loaded(false), _loading_failed (false)
{
}

void Model::render(ShaderProgram& shader)
{
    if (!_is_loaded)
        load_model(_path);

    for (const auto& mesh : _meshes)
        mesh.render(shader);
}

glm::mat4 Model::get_transformation_matrix() const
{
    glm::mat4 transformation(1.0f);

    transformation = glm::translate(transformation, _local_position);
    transformation = glm::rotate(transformation, glm::radians(_local_rotation.x), glm::vec3(1.0, 0.0, 0.0));
    transformation = glm::rotate(transformation, glm::radians(_local_rotation.y), glm::vec3(0.0, 1.0, 0.0));
    transformation = glm::rotate(transformation, glm::radians(_local_rotation.z), glm::vec3(0.0, 0.0, 1.0));
    transformation = glm::scale(transformation, glm::vec3(_local_scale));

    return transformation;
}

void Model::load_model(const std::string& path)
{
    if (_loading_failed && _last_write >= std::filesystem::last_write_time(path))
    {
        // - last loading failed and file was not updated since, don't try to load it again
        return;
    }

    Logger::add_entry(LogType::INFO, "start loading " + fs_utils::get_filename(path));
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    _last_write = std::filesystem::last_write_time(path);
    _loading_failed = true;

    if (std::string("").compare(import.GetErrorString()) != 0)
    {
        Logger::add_entry(LogType::ERROR, import.GetErrorString());
        return;
    }

    if (!scene)
    {
        Logger::add_entry(LogType::ERROR, "no scene found");
        return;
    }
    
    if (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE)
    {
        Logger::add_entry(LogType::ERROR, "incomplete scene");
        return;
    }

    if (!scene->mRootNode)
    {
        Logger::add_entry(LogType::ERROR, " scene contains no root node");
        return;
    }

    _directory = path.substr(0, path.find_last_of('\\'));

    aiVector3D scaling, position;
    aiQuaternion rotation;
    scene->mRootNode->mTransformation.Decompose(scaling, rotation, position);
    _local_scale = 1.0f;
    _local_rotation = glm::vec4(rotation.x, rotation.y, rotation.z, rotation.w);
    _local_position = glm::vec3(position.x, position.y, position.z);

    process_node(scene->mRootNode, scene);

    _is_loaded = true;
    _loading_failed = false;

    Logger::add_entry(LogType::INFO, fs_utils::get_filename(path) + " succesfully loaded");
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

        // TODO : Inverted x and z coordinates on wow model => find a way to detect axis orientation

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

        std::vector<Texture> diffuse_maps = load_material_textures(scene, material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());

        std::vector<Texture> specular_maps = load_material_textures(scene, material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(const aiScene* scene, aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < material->GetTextureCount(type); i++)
    {
        aiString filename;
        material->GetTexture(type, i, &filename);
        bool skip = false;

        for (auto const& texture_loaded : _textures_loaded)
        {
            if (std::strcmp(texture_loaded._path.data(), filename.C_Str()) == 0)
            {
                textures.push_back(texture_loaded);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;

            // embedded texture
            if ('*' == filename.data[0])
            {
                unsigned int index = std::stoi(std::string(&filename.data[1]));
                aiTexture* embedded_texture = scene->mTextures[index];

                texture._id = load_texture_from_data(embedded_texture);
            }

            // file texture
            else
            {
                texture._id = load_texture_from_file(filename.C_Str(), _directory);
            }
            
            texture._type = type_name;
            texture._path = filename.C_Str();
            textures.push_back(texture);
            _textures_loaded.push_back(texture);
        }
    }

    return textures;
}

unsigned int Model::load_texture_from_file(std::string filename, std::string directory)
{
    int width, height, nb_channels;
    unsigned char* data = stbi_load((directory + '\\'+ filename).c_str(), &width, &height, &nb_channels, 0);
    unsigned int id = 0;

    if (data)
        id = load_texture(data, width, height, nb_channels);

    else
        Logger::add_entry(LogType::WARNING, " failed to load texture " + directory + "\\" + filename);

    stbi_image_free(data);

    return id;
}

unsigned int Model::load_texture_from_data(const aiTexture* texture)
{
    int width, height, nb_channels;
    unsigned char* data = nullptr;
    unsigned int id = 0;

    if (texture->mHeight == 0)
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth, &width, &height, &nb_channels, 0);

    else
        data = stbi_load_from_memory(reinterpret_cast<unsigned char*>(texture->pcData), texture->mWidth * texture->mHeight, &width, &height, &nb_channels, 0);

    if (data)
        id = load_texture(data, width, height, nb_channels);

    else
        Logger::add_entry(LogType::WARNING, "failed to load embedded texture");

    // extract embedded texture : sometimes WMV exported fbx contains embedded textures
    // references and path to textures, but never provide these files.
    // std::string file = std::to_string(id) + ".png";
    // stbi_write_png(file.c_str(), width, height, STBI_rgb_alpha, data, 0);

    stbi_image_free(data);

    return id;
}

unsigned int Model::load_texture(unsigned char* data, int width, int height, int nb_channels) const
{
    unsigned int id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data)
    {
        if (nb_channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        else if (nb_channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    return id;
}

void Model::render_ui()
{
    ImGui::InputFloat3("Translation", &_local_position[0]);
    ImGui::InputFloat3("Rotation", &_local_rotation[0]);
    ImGui::InputFloat("Scale", &_local_scale);
}