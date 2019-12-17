#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const GLchar* vertex_path, const GLchar* fragment_path)
{
    std::string vertex_str;
    std::string fragment_str;
    std::ifstream vertex_file;
    std::ifstream fragment_file;

    // ensure ifstream objects can throw exceptions:
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_file.open(vertex_path);
        fragment_file.open(fragment_path);

        std::stringstream vertex_stream, fragment_stream;
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        vertex_str = vertex_stream.str();
        fragment_str = fragment_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n";
    }

    const char* vertex_src = vertex_str.c_str();
    const char* fragment_src = fragment_str.c_str();

    // - compile shaders

    unsigned int vertex, fragment;
    int success;
    char log[512];

    // - vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_src, nullptr);
    glCompileShader(vertex);
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(vertex, 512, nullptr, log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << log << "\n";
    }

    // - vertex
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_src, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << log << "\n";
    }

    // - linking
    _id = glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(_id, 512, nullptr, log);
        std::cout << "ERROR::SHADER_PROGRAM::LINKING::FAILED\n" << log << "\n";
    }

    // - delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void ShaderProgram::use()
{
    glUseProgram(_id);
}

void ShaderProgram::set_int(const std::string& name, int value)
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::set_float(const std::string& name, float value)
{
    glUniform1f(glGetUniformLocation(_id, name.c_str()), value);
}

void ShaderProgram::set_bool(const std::string& name, bool value)
{
    glUniform1i(glGetUniformLocation(_id, name.c_str()), (int)value);
}

void ShaderProgram::set_vec3(const std::string& name, glm::vec3 vec3)
{
    glUniform3f(glGetUniformLocation(_id, name.c_str()), vec3.x, vec3.y, vec3.z);
}

void ShaderProgram::set_mat4(const std::string& name, glm::mat4 mat4)
{
    glUniformMatrix4fv(glGetUniformLocation(_id, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat4));
}