#ifndef _SHADER_PROGRAM_H_INCLUDED_
#define _SHADER_PROGRAM_H_INCLUDED_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filesystem>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

#include "Logger.h"

class ShaderProgram
{
public:
    ShaderProgram(std::string name, std::string vertex_path, std::string fragment_path);

    void compile();
    void use();
    void set_bool(const std::string& name, bool value);
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec3(const std::string& name, glm::vec3 value);
    void set_mat4(const std::string& name, glm::mat4 mat4);
    
    inline const std::string& get_name() const { return _name; }
    inline const std::string& get_vertex_path() const { return _vertex_path; }
    inline const std::string& get_fragment_path() const { return _fragment_path; }
    inline const std::filesystem::file_time_type& get_last_vertex_write() const { return _last_vertex_write; }
    inline const std::filesystem::file_time_type& get_last_fragment_write() const { return _last_fragment_write; }
    inline const unsigned int get_id() const { return _id; }

private:
    unsigned int _id;
    std::string _name;
    std::string _vertex_path;
    std::string _fragment_path;
    std::filesystem::file_time_type _last_vertex_write;
    std::filesystem::file_time_type _last_fragment_write;
};

#endif