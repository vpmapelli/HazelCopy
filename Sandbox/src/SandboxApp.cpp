#include <Hazel.h>

#include "Platform/OpenGL/OpenGLShader.h"
// #include "Platform/OpenGL/OpenGLTexture.h"

#include <imgui/imgui.h>

#include <hzpch.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6, -0.9f, 0.9f), m_CameraPosition(0.0f), m_SquarePosition(0.0f)
        { 

            m_VertexArray.reset(Hazel::VertexArray::Create());

            float vertices[3 * 7] = {
                   -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                    0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                    0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
            };

            Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
            vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
            Hazel::BufferLayout layout = {
                { Hazel::ShaderDataType::Float3, "a_Position"},
                { Hazel::ShaderDataType::Float4, "a_Color"},
            };
            vertexBuffer->SetLayout(layout);

            m_VertexArray->AddVertexBuffer(vertexBuffer);

            unsigned int indices[3] = { 0, 1, 2 };
            Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
            indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t) ));
            m_VertexArray->SetIndexBuffer(indexBuffer);

            m_SquareVA.reset(Hazel::VertexArray::Create());

            float squareVertices[5 * 4] = {
                -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
                 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
                 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
                -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
            };

            Hazel::Ref<Hazel::VertexBuffer> squareVB;
            squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
            layout = {
                { Hazel::ShaderDataType::Float3, "a_Position"},
                { Hazel::ShaderDataType::Float2, "a_TexCoords"}
            };
            squareVB->SetLayout(layout);
            m_SquareVA->AddVertexBuffer(squareVB);

            unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
            Hazel::Ref<Hazel::IndexBuffer> squareIB;
            squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t) ));
            m_SquareVA->SetIndexBuffer(squareIB);

            std::string vertexSrc = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec4 a_Color;

                uniform mat4 u_ViewProjectionMatrix;

                out vec3 v_Position;
                out vec4 v_Color;

                void main()
                {
                    v_Position = a_Position;
                    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
                    v_Color = a_Color;
                }

            )";

            std::string fragmentSrc = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                in vec3 v_Position;
                in vec4 v_Color;

                void main()
                {
                    color = vec4(v_Position * 0.5 + 0.5, 1.0);
                    color = v_Color;
                }

            )";

            m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


            std::string flatColorVertexSrc = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;
                layout(location = 1) in vec2 a_TexCoords;

                uniform mat4 u_ViewProjectionMatrix;
                uniform mat4 u_Transform;

                out vec3 v_Position;

                void main()
                {
                    v_Position = a_Position;
                    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
                }

            )";

            std::string flatColorFragmentSrc = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                in vec3 v_Position;
                uniform vec3 u_Color;

                void main()
                {
                    color = vec4(u_Color, 1.0f);
                }

            )";

            m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorVertexSrc, flatColorFragmentSrc);

            auto textureShader = m_ShaderLibrary.Load("./assets/shaders/TextureShader.glsl");

            m_Texture = Hazel::Texture2D::Create("./assets/textures/CheckerBoard.png");
            m_ChernoLogoTexture = Hazel::Texture2D::Create("./assets/textures/ChernoLogo.png");

            std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
            std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
        }        

    void OnUpdate(Hazel::Timestep ts) override
    {
        HZ_TRACE("Timestep {0}(s), {1}(ms)", ts.GetSeconds(), ts.GetMiliseconds());


        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();

        if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * ts;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * ts;

        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        Hazel::Renderer::BeginScene(m_Camera);

        glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        // As shown, it also works without dynamic casting.
        // m_FlatColorShader->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int j=0; j<20; j++)
        {
           for (int i=0; i<20; i++) 
            {
                glm::vec3 pos(0.11f * i, 0.11f * j, 1.0f);
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
                Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
            }
        }

        auto textureShader = m_ShaderLibrary.Get("TextureShader");

        //Both lines below work. Not still sure why, but dynamic casting seems more correct(?)
        // Cherno uses without dynamic casting for textures. In this case, it is not needed to included
        // OpenGLTexture.h file
        m_Texture->Bind();
        //  std::dynamic_pointer_cast<Hazel::OpenGLTexture2D>(m_Texture)->Bind();

        Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f) ) );

        m_ChernoLogoTexture->Bind();
        Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f) ) );


        // Triangle
        // Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        Hazel::Renderer::EndScene();
    }

    void OnEvent(Hazel::Event& event) override
    {
        // HZ_TRACE("{0}", event);
    }

    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }

private:
    Hazel::ShaderLibrary m_ShaderLibrary;
    Hazel::Ref<Hazel::Shader> m_Shader;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;

    Hazel::Ref<Hazel::Shader> m_FlatColorShader;
    Hazel::Ref<Hazel::VertexArray> m_SquareVA;
    Hazel::Ref<Hazel::Texture2D> m_Texture;
    Hazel::Ref<Hazel::Texture2D> m_ChernoLogoTexture;

    Hazel::OrthographicCamera m_Camera;
    glm::vec3 m_CameraPosition;
    glm::vec3 m_SquarePosition;
    float m_CameraMoveSpeed = 1.0f;

    float m_CameraRotation = 0.0f;
    float m_CameraRotationSpeed = 180.0f;

    glm::vec3 m_SquareColor = { 0.2f, 0.5f, 0.8f };
};
class Sandbox : public Hazel::Application
{
public:
        Sandbox()
        {
            PushLayer(new ExampleLayer());
        }

        ~Sandbox()
        {

        }
};

Hazel::Application* Hazel::CreateApplication()
{
        return new Sandbox();
}