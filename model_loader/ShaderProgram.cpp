#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(std::string name, std::string vertex_path, std::string fragment_path) :
    _name(name), _vertex_path(vertex_path), _fragment_path(fragment_path), _id(0)
{
    compile();
}

void ShaderProgram::compile()
{
    Logger::add_entry(LogType::INFO, "start " + _name + " compilation");

    std::string str_vertex_src;
    std::string str_fragment_src;
    std::ifstream vertex_file;
    std::ifstream fragment_file;

    // ensure ifstream objects can throw exceptions:
    vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_file.open(_vertex_path);
        fragment_file.open(_fragment_path);

        std::stringstream vertex_stream, fragment_stream;
        vertex_stream << vertex_file.rdbuf();
        fragment_stream << fragment_file.rdbuf();

        vertex_file.close();
        fragment_file.close();

        str_vertex_src = vertex_stream.str();
        str_fragment_src = fragment_stream.str();
    }
    catch (std::ifstream::failure e)
    {
        Logger::add_entry(LogType::ERROR, "failed to read " + _name + " sources files");
    }

    // - compile shaders

    const char* vertex_src = str_vertex_src.c_str();
    const char* fragment_src = str_fragment_src.c_str();
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
        Logger::add_entry(LogType::ERROR, "vertex compilation failed " + std::string(log));
    }

    // - vertex
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_src, nullptr);
    glCompileShader(fragment);
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(fragment, 512, nullptr, log);
        std::cout << "fragment compilation failed " + std::string(log);
    }

    // - linking
    // - delete old version

    if (_id != 0)
        glDeleteProgram(_id);

    _id =  glCreateProgram();
    glAttachShader(_id, vertex);
    glAttachShader(_id, fragment);
    glLinkProgram(_id);
    glGetProgramiv(_id, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(_id, 512, nullptr, log);
        std::cout << "linking failed " + std::string(log);
    }

    _last_vertex_write = std::filesystem::last_write_time(_vertex_path);
    _last_fragment_write = std::filesystem::last_write_time(_fragment_path);

    Logger::add_entry(LogType::INFO, _name + " successfully compiled");

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