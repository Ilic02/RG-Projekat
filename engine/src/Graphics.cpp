
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine/controller/ControllerManager.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/platform/PlatformController.hpp>
#include <engine/resources/Skybox.hpp>

namespace engine::graphics {

    void GraphicsController::initialize() {
        const int opengl_initialized = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
        RG_GUARANTEE(opengl_initialized, "OpenGL failed to init!");

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        auto platform               = engine::controller::get<platform::PlatformController>();
        auto handle                 = platform->window()->handle_();
        m_perspective_params.FOV    = glm::radians(m_camera.Zoom);
        m_perspective_params.Width  = static_cast<float>(platform->window()->width());
        m_perspective_params.Height = static_cast<float>(platform->window()->height());
        m_perspective_params.Near   = 0.1f;
        m_perspective_params.Far    = 100.f;

        m_ortho_params.Bottom = 0.0f;
        m_ortho_params.Top    = static_cast<float>(platform->window()->height());
        m_ortho_params.Left   = 0.0f;
        m_ortho_params.Right  = static_cast<float>(platform->window()->width());
        m_ortho_params.Near   = 0.1f;
        m_ortho_params.Far    = 100.0f;

        platform->register_platform_event_observer(
                std::make_unique<GraphicsPlatformEventObserver>(this));
        ImGui_ImplGlfw_InitForOpenGL(handle, true);
        ImGui_ImplOpenGL3_Init("#version 330 core");
    }

    void GraphicsPlatformEventObserver::on_window_resize(int width, int height) {
        m_graphics->perspective_params().Width  = static_cast<float>(width);
        m_graphics->perspective_params().Height = static_cast<float>(height);

        m_graphics->orthographic_params().Right = static_cast<float>(width);
        m_graphics->orthographic_params().Top   = static_cast<float>(height);
    }

    std::string_view GraphicsController::name() const {
        return "GraphicsController";
    }

    void GraphicsController::begin_gui() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void GraphicsController::end_gui() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void GraphicsController::draw_skybox(const resources::Shader *shader, const resources::Skybox *skybox) {
        glm::mat4 view = glm::mat4(glm::mat3(m_camera.view_matrix()));
        shader->use();
        shader->set_mat4("view", view);
        shader->set_mat4("projection", projection_matrix<>());
        glDepthFunc(GL_LEQUAL);
        glBindVertexArray(skybox->vao());
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->texture());
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    }
}
