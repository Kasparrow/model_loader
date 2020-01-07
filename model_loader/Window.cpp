#include "Window.h"

Window::Window(std::string name, int width, int height)
    : _name{ name }, _width{ width }, _height{ height }, _delta{ 0 }, _last_frame{ 0 },
    _camera{ glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
    _cursor_info{ width / 2.0f, height / 2.0f, true, false, true }, _input_info { false },
    _gl_parameters{ false, true, true }, _window{ nullptr }
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

    glViewport(0, 0, _width, _height);

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
        static_cast<Window*>(glfwGetWindowUserPointer(window))->on_resize(window, width, height);
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

void Window::process_inputs()
{
    float delta = get_delta();
    
    _cursor_info._was_locked = _cursor_info._is_locked;
       
    if (!_input_info._lock_movement)
    {
        if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS)
            _camera.process_keyboard(FORWARD, delta);

        if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS)
            _camera.process_keyboard(BACKWARD, delta);

        if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS)
            _camera.process_keyboard(LEFT, delta);

        if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS)
            _camera.process_keyboard(RIGHT, delta);
    }

    if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || _input_info._lock_movement)
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        _cursor_info._is_locked = false;
    }
    else
    {
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        _cursor_info._is_locked = true;
    }
}

void Window::update_delta()
{
    float current_frame = glfwGetTime();

    _delta = current_frame - _last_frame;
    _last_frame = current_frame;
}

void Window::apply_gl_parameters() const
{
    if (_gl_parameters._is_wire_mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (_gl_parameters._use_depth)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH);

    if (_gl_parameters._use_blend)
        glEnable(GL_BLEND);
    else
        glDisable(GL_DEPTH);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::on_resize(GLFWwindow* window, int width, int height)
{
    _width = width;
    _height = height;
    glViewport(0, 0, width, height);
}

void Window::on_mouse(GLFWwindow* window, double x_pos, double y_pos)
{
    // - reinit cursor on unlock
    if (!_cursor_info._was_locked || _cursor_info._init)
    {
        _cursor_info._last_x = x_pos;
        _cursor_info._last_y = y_pos;
        _cursor_info._init = false;
    }

    float x_offset = x_pos - _cursor_info._last_x;
    float y_offset = _cursor_info._last_y - y_pos;
    _cursor_info._last_x = x_pos;
    _cursor_info._last_y = y_pos;

    if (!_cursor_info._is_locked)
        return;

    _camera.process_mouse(x_offset, y_offset);
}

void Window::render_ui()
{
    std::string fps = "FPS : " + std::to_string(1.0f / get_delta());
    std::string camera_position = "Camera position : " + str_utils::vec3_to_string(_camera.get_position());
    std::string camera_orientation = "Camera orientation : " + str_utils::vec3_to_string(_camera.get_front());

    ImGui::NewFrame();
    ImGui::Begin("Debug");
    ImGui::Text(fps.c_str());
    ImGui::Text(camera_position.c_str());
    ImGui::Text(camera_orientation.c_str());
    ImGui::Checkbox("Wiremode", &_gl_parameters._is_wire_mode); ImGui::SameLine();
    ImGui::Checkbox("Enable GL_DEPTH", &_gl_parameters._use_depth); ImGui::SameLine();
    ImGui::Checkbox("Enable GL_BLEND", &_gl_parameters._use_blend);
    ImGui::End();
}