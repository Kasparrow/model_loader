#ifndef _SHADER_MANAGER_H_INCLUDED_
#define _SHADER_MANAGER_H_INCLUDED_

#include <algorithm>
#include <vector>

#include "ShaderProgram.h"
#include "Utils.h"

class ShaderManager
{
    public:
        ShaderManager();
        void initialize();
        void scan_folders();
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