#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>

#include "Window.h"
#include "ShaderManager.h"
#include "LightManager.h"
#include "ModelManager.h"

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
    
    Window window("Model loader", 1600, 1200);
    window.initialize();
    
    ShaderManager shader_manager;
    shader_manager.initialize();
    
    LightManager light_manager;
    light_manager.add_directional(glm::vec3(0.0f, -1.0f, 0.0f));

    ModelManager model_manager;
    model_manager.initialize();


    while (window.is_open())
    {
        window.update_delta();
        window.process_inputs();
        window.apply_gl_parameters();

        // - clear
        glClearColor(BACKGROUND_COLOR);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ShaderProgram& shader = shader_manager.get_current_shader();
        shader.use();
        shader.set_mat4("model", model_manager.get_current_model().get_transformation_matrix());
        shader.set_mat4("projection", glm::perspective(glm::radians(45.0f), window.get_ratio(), 0.1f, 100.0f));
        shader.set_mat4("view", window.get_camera().get_view_matrix());
        shader.set_vec3("view_position", window.get_camera().get_position());
        light_manager.set_uniforms(shader);

        model_manager.render(shader);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        window.render_ui();
        model_manager.render_ui();
        shader_manager.render_ui();
        light_manager.render_ui();
        Logger::render_ui();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        window.lock_movement(ImGui::GetIO().WantCaptureKeyboard);
        window.swap_buffers();
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}