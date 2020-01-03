#include "Window.h"

Window::Window(std::string name, int width, int height)
    : _name(name), _width(width), _height(height), _delta(0), _last_frame(0),
    _camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
    _last_x(width / 2), _last_y(height / 2), _mouse_init(true), _was_cursor_locked(false),
    _is_cursor_locked(true), _window(nullptr)
{
}

Window::~Window()
{
    // nothing to do, _window is delete with glfwTerminate at the end of the program
}

void Window::initialize()
{
    if (!initialize_glfw())
    {
        std::cout << "Failed to initialize GLFW.\n";
        glfwTerminate();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD.\n";
        glfwTerminate();
    }

    initialize_imgui();

    assert(_window != nullptr);
}

bool Window::initialize_glfw()
{
    // - create window
    _window = glfwCreateWindow(_width, _height, _name.c_str(), nullptr, nullptr);

    if (_window == nullptr)
    {
        std::cout << "Failed to create GLFW window.\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(_window);

    // - set callbacks
    // - we can't use pointer to method in glfw callbacks, see 
    // https://stackoverflow.com/questions/7676971/pointing-to-a-function-that-is-a-class-member-glfw-setkeycallback
    // - the idea is to define a user for the GLFWwindow and call the member function of the user in the callback
    glfwSetWindowUserPointer(_window, this);

    auto mouse_callback = [](GLFWwindow* window, double x_pos, double y_pos)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->on_mouse(window, x_pos, y_pos);
    };

    auto resize_callback = [](GLFWwindow* window, int width, int height)
    {
        static_cast<Window*>(glfwGetWindowUserPointer(window))->on_mouse(window, width, height);
    };

    glfwSetFramebufferSizeCallback(_window, resize_callback);
    glfwSetCursorPosCallback(_window, mouse_callback);

    return true;
}

void Window::initialize_imgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

bool Window::is_open()
{
    return !glfwWindowShouldClose(_window);
}

void Window::process_inputs()
{
    float delta = get_delta();
    
    _was_cursor_locked = _is_cursor_locked;
       
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
        _camera.process_keyboard(FORWARD, delta);

    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
        _camera.process_keyboard(BACKWARD, delta);

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
        _camera.process_keyboard(LEFT, delta);

    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
        _camera.process_keyboard(RIGHT, delta);

    if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _is_cursor_locked = false;
    }
    else
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        _is_cursor_locked = true;
    }
}

void Window::update_delta()
{
    float current_frame = glfwGetTime();

    _delta = current_frame - _last_frame;
    _last_frame = current_frame;
}

float Window::get_delta()
{
    return _delta;
}

void Window::on_resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::on_mouse(GLFWwindow* window, double x_pos, double y_pos)
{
    // - reinit cursor on unlock
    if (!_was_cursor_locked || _mouse_init)
    {
        _last_x = x_pos;
        _last_y = y_pos;
        _mouse_init = false;
    }

    float x_offset = x_pos - _last_x;
    float y_offset = _last_y - y_pos;
    _last_x = x_pos;
    _last_y = y_pos;

    if (!_is_cursor_locked)
        return;

    _camera.process_mouse(x_offset, y_offset);
}

void Window::swap_buffers()
{
    glfwSwapBuffers(_window);
}

void Window::close()
{
    glfwSetWindowShouldClose(_window, true);
}