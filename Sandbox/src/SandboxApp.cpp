#include <Hazel.h>

#include <hzpch.h>

class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_Camera(-1.6f, 1.6, -0.9f, 0.9f) 
        { 

            m_VertexArray.reset(Hazel::VertexArray::Create());

            float vertices[3 * 7] = {
                -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
                    0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
                    0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f,
            };

            std::shared_ptr<Hazel::VertexBuffer> vertexBuffer;
            vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
            Hazel::BufferLayout layout = {
                { Hazel::ShaderDataType::Float3, "a_Position"},
                { Hazel::ShaderDataType::Float4, "a_Color"},
            };
            vertexBuffer->SetLayout(layout);

            m_VertexArray->AddVertexBuffer(vertexBuffer);

            unsigned int indices[3] = { 0, 1, 2 };
            std::shared_ptr<Hazel::IndexBuffer> indexBuffer;
            indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t) ));
            m_VertexArray->SetIndexBuffer(indexBuffer);

            m_SquareVA.reset(Hazel::VertexArray::Create());

            float squareVertices[3 * 4] = {
                -0.75f, -0.75f, 0.0f,
                 0.75f, -0.75f, 0.0f,
                 0.75f,  0.75f, 0.0f,
                -0.75f,  0.75f, 0.0f,
         };
            std::shared_ptr<Hazel::VertexBuffer> squareVB;
            squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
            squareVB->SetLayout( {
                { Hazel::ShaderDataType::Float3, "a_Position"},
            });
            m_SquareVA->AddVertexBuffer(squareVB);

            unsigned int squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
            std::shared_ptr<Hazel::IndexBuffer> squareIB;
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

            m_Shader.reset(new Hazel::Shader(vertexSrc, fragmentSrc));


            std::string blueSquareVertexSrc = R"(
                #version 330 core

                layout(location = 0) in vec3 a_Position;

                uniform mat4 u_ViewProjectionMatrix;

                out vec3 v_Position;
                out vec4 v_Color;

                void main()
                {
                    v_Position = a_Position;
                    gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
                }

            )";

            std::string blueSquarefragmentSrc = R"(
                #version 330 core

                layout(location = 0) out vec4 color;

                in vec3 v_Position;
                in vec4 v_Color;

                void main()
                {
                    color = vec4(0.2, 0.3, 0.8, 1.0);
                }

            )";

            m_BlueShader.reset(new Hazel::Shader(blueSquareVertexSrc, blueSquarefragmentSrc));


            m_Camera.SetPosition({0.5f, 0.5f, 0.0f});
            // m_Camera.SetRotation(45.0f);
        }        

    void OnUpdate() override
    {
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();


        glm::vec3 position = m_Camera.GetPosition();
        float rotation = m_Camera.GetRotation();

        float velocity = 0.1f;
        float omega = 5.0f;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            position.x += velocity;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            position.x -= velocity;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            position.y += velocity;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            position.y -= velocity;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            rotation += omega;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            rotation -= omega;

        m_Camera.SetPosition(position);
        m_Camera.SetRotation(rotation);

        Hazel::Renderer::BeginScene(m_Camera);

        Hazel::Renderer::Submit(m_BlueShader, m_SquareVA);
        Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        Hazel::Renderer::EndScene();
    }

    void OnEvent(Hazel::Event& event) override
    {
        // HZ_TRACE("{0}", event);
    }

private:
    std::shared_ptr<Hazel::Shader> m_Shader;
    std::shared_ptr<Hazel::VertexArray> m_VertexArray;

    std::shared_ptr<Hazel::Shader> m_BlueShader;
    std::shared_ptr<Hazel::VertexArray> m_SquareVA;

    Hazel::OrthographicCamera m_Camera;
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