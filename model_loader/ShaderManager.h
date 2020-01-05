#ifndef _SHADER_MANAGER_H_INCLUDED_
#define _SHADER_MANAGER_H_INCLUDED_

#include <algorithm>
#include <vector>

#include "ShaderProgram.h"
#include "Utils.h"

#define SHADER_FOLDER std::string("..\\resources\\shaders\\")
#define DEFAULT_NAME "Default"
#define DEFAULT_VERTEX SHADER_FOLDER + "default.vert"
#define DEFAULT_FRAGMENT SHADER_FOLDER + "default.frag"

class ShaderManager
{
    public:
        ShaderManager();
        void initialize();
        void scan_shader_folder();
        void recompile_shaders();
        void add_shader(const ShaderProgram& shader);
        void render_ui();

        inline ShaderProgram& get_current_shader() { return _shaders[_current_index]; };

    private:
        std::vector<ShaderProgram> _shaders;
        std::vector<std::string> _vertex_paths;
        std::vector<std::string> _fragment_paths;
        unsigned int _current_index;
        unsigned int _new_vertex;
        unsigned int _new_fragment;
        std::string _new_name;
};

#endif