#ifndef _SHADER_PROGRAM_H_INCLUDED_
#define _SHADER_PROGRAM_H_INCLUDED_

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram
{
public:
    ShaderProgram(const GLchar* vertex_path, const GLchar* fragment_path);

    void use();
    void set_bool(const std::string& name, bool value);
    void set_int(const std::string& name, int value);
    void set_float(const std::string& name, float value);
    void set_vec3(const std::string& name, glm::vec3 value);
    void set_mat4(const std::string& name, glm::mat4 mat4);

private:
    unsigned int _id;
};

#endif