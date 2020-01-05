#ifndef _WINDOW_H_INCLUDED_
#define _WINDOW_H_INCLUDED_

#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Utils.h"
#include "Camera.h"

#include <GLFW/glfw3.h>

#define BACKGROUND_COLOR 0.01, 0.01f, 0.01f, 1.0f

struct CursorInfo
{
    float _last_x;
    float _last_y;
    bool _init;
    bool _was_locked;
    bool _is_locked;
};

struct GLParameters
{
    bool _is_wire_mode;
    bool _use_depth;
    bool _use_blend;
};

class Window
{
public:
    Window(std::string name = "Window", int width = 800, int height = 600);
    ~Window();

    void initialize();
    void process_inputs();
    void update_delta();
    void render_ui();

    void apply_gl_parameters() const;
    
    inline void swap_buffers() const { glfwSwapBuffers(_window); }
    inline void close() const { glfwSetWindowShouldClose(_window, true); }
    inline bool is_open() const { return !glfwWindowShouldClose(_window); }

    inline int get_width() const { return _width; }
    inline int get_height() const { return _height; }
    inline float get_ratio() const { return static_cast<float>(_width) / static_cast<float>(_height); }
    inline float get_delta() const { return _delta; };
    inline const Camera& get_camera() const { return _camera; }

private:
    GLFWwindow* _window;
    Camera _camera;
    std::string _name;
    int _width;
    int _height;
    float _last_frame;
    float _delta;

    CursorInfo _cursor_info;
    GLParameters _gl_parameters;

    bool initialize_glfw();
    void initialize_imgui();
    void on_resize(GLFWwindow* window, int width, int height);
    void on_mouse(GLFWwindow* window, double x_pos, double y_pos);
};

#endif