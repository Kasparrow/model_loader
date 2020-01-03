#ifndef _WINDOW_H_INCLUDED_
#define _WINDOW_H_INCLUDED_

#include <string>
#include <iostream>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Camera.h"

#include <GLFW/glfw3.h>

class Window
{
public:
    Window(std::string name = "Window", int width = 800, int height = 600);
    ~Window();

    void initialize();
    bool is_open();
    void process_inputs();
    void update_delta();
    float get_delta();
    void swap_buffers();
    void close();

    inline int get_width() { return _width; }
    inline int get_height() { return _height; }
    inline Camera& get_camera() { return _camera; }

private:
    GLFWwindow* _window;
    Camera _camera;
    std::string _name;
    int _width;
    int _height;

    float _last_frame;
    float _delta;

    float _last_x;
    float _last_y;
    bool _mouse_init;
    bool _was_cursor_locked;
    bool _is_cursor_locked;

    bool initialize_glfw();
    void initialize_imgui();

    // - callbacks
    void on_resize(GLFWwindow* window, int width, int height);
    void on_mouse(GLFWwindow* window, double x_pos, double y_pos);
};

#endif