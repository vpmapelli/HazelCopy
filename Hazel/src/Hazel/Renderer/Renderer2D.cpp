#include "hzpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"

#include "RenderCommand.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel
{

    struct Renderer2DStorage
    {
        Ref<VertexArray> QuadVertexArray;
        Ref<Shader> TextureShader;
        Ref<Texture2D> WhiteTexture;
    };

    static Renderer2DStorage *s_Data;

    void Renderer2D::Init()
    {
        s_Data = new Renderer2DStorage();
        s_Data->QuadVertexArray = VertexArray::Create();

        float squareVertices[5 * 4] = {
           -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
            0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
           -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        Ref<VertexBuffer> squareVB;
        squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

        squareVB->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float2, "a_TexCoords"}
        });
        s_Data->QuadVertexArray->AddVertexBuffer(squareVB);

        unsigned int squareIndices[6] = {0, 1, 2, 2, 3, 0};
        Ref<IndexBuffer> squareIB;
        squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
        s_Data->QuadVertexArray->SetIndexBuffer(squareIB);

        s_Data->WhiteTexture = Texture2D::Create(1, 1);
        uint32_t whiteTextureData = 0xffffffff;
        s_Data->WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

        s_Data->TextureShader = Shader::Create("assets/shaders/Texture.glsl");
        s_Data->TextureShader->Bind();
        s_Data->TextureShader->SetInt("u_Texture", 0);
    }

        void Renderer2D::Shutdown()
        {
            delete s_Data;
        }

        void Renderer2D::BeginScene(const OrthographicCamera &camera)
        {
            s_Data->TextureShader->Bind();
            s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

            s_Data->TextureShader->Bind();
            s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
        }

        void Renderer2D::EndScene()
        {
        }

        void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation)
        {
            DrawQuad({position.x, position.y, 0.0f}, size, color, rotation);
        }

        void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const glm::vec4 &color, float rotation)
        {
            s_Data->TextureShader->Bind();
            s_Data->TextureShader->SetFloat4("u_Color", color);

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
                                  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            s_Data->TextureShader->SetMat4("u_Transform", transform);

            s_Data->WhiteTexture->Bind();

            s_Data->QuadVertexArray->Bind();
            RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
        }

        void Renderer2D::DrawQuad(const glm::vec2 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float rotation)
        {
            DrawQuad({position.x, position.y, 0.0f}, size, texture, rotation);
        }

        void Renderer2D::DrawQuad(const glm::vec3 &position, const glm::vec2 &size, const Ref<Texture2D> &texture, float rotation)
        {
            s_Data->TextureShader->Bind();
            s_Data->TextureShader->SetFloat4("u_Color", glm::vec4(1.0f));

            glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                  glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
                                  glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
            s_Data->TextureShader->SetMat4("u_Transform", transform);

            texture->Bind();

            s_Data->QuadVertexArray->Bind();
            RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
        }
}
