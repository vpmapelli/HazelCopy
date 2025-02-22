#include "Sandbox2D.h"
#include <imgui/imgui.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>


Sandbox2D::Sandbox2D() :
    Layer("Sandbox2D"), m_CameraController(1280.0f/720.0f)
{}

void Sandbox2D::OnAttach()  
{
    m_CheckerBoardTexture = Hazel::Texture2D::Create("./assets/textures/CheckerBoard.png");
}

void Sandbox2D::OnDetach()
{
    
}

#define Time(name) Timer timer##__LINE__(name, [&](ProfilingResult result){ m_ProfilingResults.push_back(result); })

void Sandbox2D::OnUpdate(Hazel::Timestep ts)  
{
    // Timer timer("Sandbox2D::OnUpdate", [&](ProfilingResult result){ m_ProfilingResults.push_back(result); });
    HZ_PROFILE_FUNCTION();
    //Update
    {
        HZ_PROFILE_SCOPE("CameraController::OnCameraUpdate");
        m_CameraController.OnUpdate(ts);
    }

        // Render
	{
        HZ_PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();
    }
    {
        HZ_PROFILE_SCOPE("Renderer Draw");
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
        Hazel::Renderer2D::DrawQuad({-1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Hazel::Renderer2D::DrawQuad({0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
        Hazel::Renderer2D::DrawQuad({0.0f, 0.0f, -0.1f}, {10.0f, 10.0f}, m_CheckerBoardTexture);
        Hazel::Renderer2D::EndScene();
    }

    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
    // std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);

    // Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

}

void Sandbox2D::OnEvent(Hazel::Event& event) {
    m_CameraController.OnEvent(event); 
}

void Sandbox2D::OnImGuiRender() {
    HZ_PROFILE_FUNCTION();

    ImGui::Begin("Settings");
        ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End(); 
}
