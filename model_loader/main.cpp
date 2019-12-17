#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "Window.h"
#include "LightManager.h"
#include "Model.h"

bool initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return true;
}

int main(int argc, char** argv)
{
    if (!initialize())
        return EXIT_FAILURE;

    Window window("Model loader", 1200, 800);
    
    if (!window.initialize())
        return EXIT_FAILURE;

    bool is_wire_mode = false;
    LightManager light_manager;
    Camera& camera = window.get_camera();
    light_manager.add_directional(glm::vec3(0.0f, -1.0f, 0.0f));

    Model nanosuit("..\\resources\\models\\nanosuit\\nanosuit.obj");

    glViewport(0, 0, window.get_width(), window.get_height());

    ShaderProgram shader("object.vert", "object.frag");

    glm::mat4 model(1.0f);
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.get_width() / (float)window.get_height(), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    while (window.is_open())
    {
        window.update_delta();
        window.process_inputs();

        if (is_wire_mode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // - clear
        glClearColor(0.01f, 0.01F, 0.01f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // - set shader
        shader.use();
        shader.set_mat4("model", model);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", window.get_camera().get_view_matrix());
        shader.set_vec3("view_position", camera.get_position());
        light_manager.set_uniforms(shader);

        // - render model
        nanosuit.render(shader);

        // - render UI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        ImGui::Begin("Debug");
        float fps_counter = 1.0F / window.get_delta();
        std::string fps = "FPS : " + std::to_string(fps_counter);
        std::string camera_position = "Camera : (" + std::to_string(camera.get_position()[0]) + ", " +
            std::to_string(camera.get_position()[1]) + ", " + std::to_string(camera.get_position()[2]) + ")";
        ImGui::Text(fps.c_str());
        ImGui::Text(camera_position.c_str());
        ImGui::Checkbox("Wiremode", &is_wire_mode);
        ImGui::End();

        ImGui::Begin("Light Manager");
        light_manager.render_ui();
        ImGui::End();

        ImGui::ShowExampleMenuFile();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;


}