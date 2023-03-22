#pragma once

#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{ 
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(Hazel::Timestep ts) override;
    virtual void OnEvent(Hazel::Event& event) override;
    virtual void OnImGuiRender() override;
private:
    Hazel::OrthographicCameraController m_CameraController;

    //Temp
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Shader> m_FlatColorShader;

    Hazel::Ref<Hazel::Texture2D> m_CheckerBoardTexture;

    struct ProfilingResult {
      const char* Name;
      float Result;
    };

    std::vector<ProfilingResult> m_ProfilingResults;
    
    glm::vec4 m_SquareColor = { 0.2f, 0.5f, 0.8f, 1.0f };
};
