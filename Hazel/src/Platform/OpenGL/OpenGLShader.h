#pragma once

#include "Hazel/Renderer/Shader.h"
#include <glm/glm.hpp>

// TODO: REMOVE
typedef unsigned int GLenum;

namespace Hazel {

    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        OpenGLShader(const std::string& filepath);
        virtual ~OpenGLShader();

        virtual void Bind() const;
        virtual void Unbind() const;

        virtual const std::string& GetName() const override { return m_Name; }

        void SetInt(const std::string& name,int value) override;
        void SetFloat3(const std::string& name,const glm::vec3& value) override;
        void SetFloat4(const std::string& name,const glm::vec4& value) override;
        void SetMat4(const std::string& name,const glm::mat4& value) override;

        void UploadUniformInt(const std::string& name, int value);

        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
    private:
        std::string ReadFile(const std::string& filepath);
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
    private:
        uint32_t m_RendererID;
        std::string m_Name;
    };
}