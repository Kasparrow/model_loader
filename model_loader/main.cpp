#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION


#include <iostream>

#include "Window.h"
#include "ShaderManager.h"
#include "LightManager.h"
#include "Model.h"

void initialize()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

int main(int argc, char** argv)
{
    initialize();
    
    bool is_wire_mode = false;

    Window window("Model loader", 1200, 800);
    window.initialize();
    
    ShaderManager shader_manager;
    shader_manager.initialize();
    
    LightManager light_manager;
    light_manager.add_directional(glm::vec3(0.0f, -1.0f, 0.0f));

    //Model nanosuit("..\\resources\\wow_models\\kasparrow\\kasparrow.obj");
    //Model nanosuit("..\\resources\\wow_models\\alexstrasza\\alexstrasza.fbx");
    Model nanosuit("..\\resources\\models\\nanosuit\\nanosuit.obj");
    Camera& camera = window.get_camera();

    glViewport(0, 0, window.get_width(), window.get_height());

    glm::mat4 model(1.0f);

    model = glm::scale(model, nanosuit.get_local_scale());
    model = glm::translate(model, nanosuit.get_local_position());
    model = glm::rotate(model, nanosuit.get_local_rotation().x, glm::vec3(1.0, 0.0, 0.0));
    model = glm::rotate(model, nanosuit.get_local_rotation().y, glm::vec3(0.0, 1.0, 0.0));
    model = glm::rotate(model, nanosuit.get_local_rotation().z, glm::vec3(0.0, 0.0, 1.0));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window.get_width() / (float)window.get_height(), 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
        ShaderProgram& shader = shader_manager.get_current_shader();
        shader.use();
        shader.set_mat4("model", model);
        shader.set_mat4("projection", projection);
        shader.set_mat4("view", camera.get_view_matrix());
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

        ImGui::Begin("Shader");
        shader_manager.render();
        ImGui::End();

        ImGui::Begin("Light Manager");
        light_manager.render_ui();
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;


}