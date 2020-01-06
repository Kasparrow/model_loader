#ifndef _MODEL_MANAGER_H_INCLUDED_
#define _MODEL_MANAGER_H_INCLUDED_

#include <vector>

#include "Utils.h"
#include "Model.h"

class ModelManager
{
public:
    ModelManager();
    
    void initialize();
    void scan_folders();
    void render(ShaderProgram& shader);
    void render_ui();

    inline const Model& get_current_model() { return _models[_current_index]; }

private:
    unsigned int _current_index;
    std::vector<Model> _models;
};

#endif