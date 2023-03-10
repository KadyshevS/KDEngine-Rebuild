#pragma once

#include <KDEngine/Renderer/Shader.h>

#include <glm/glm.hpp>

#include <unordered_map>

typedef unsigned int GLenum;

namespace KDE
{
	class OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filePath);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual const std::string& GetName() const override { return m_Name; }

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetIntArray(const std::string& name, int* value, uint32_t count) override;
		virtual void SetInt2(const std::string& name, const glm::vec2& value) override;
		virtual void SetInt3(const std::string& name, const glm::vec3& value) override;
		virtual void SetInt4(const std::string& name, const glm::vec4& value) override;

		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void SetMat2(const std::string& name, const glm::mat2& value) override;
		virtual void SetMat3(const std::string& name, const glm::mat3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* value, uint32_t count);
		void UploadUniformInt2(const std::string& name, const glm::vec2& value);
		void UploadUniformInt3(const std::string& name, const glm::vec3& value);
		void UploadUniformInt4(const std::string& name, const glm::vec4& value);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat2(const std::string& name, const glm::mat2& matrix);
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> ParseShaderCode(const std::string& source);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	protected:
		uint32_t m_RendererID;
		std::string m_Name;
	};
}