#include <kdpch.h>
#include "OpenGLShader.h"

#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace KDE
{
	OpenGLShader::OpenGLShader(const std::string& filePath)
	{
		std::ifstream stream(filePath);
		KD_CORE_ASSERT(stream.is_open(), "Failed to open shader file.");

		enum class ShaderType
		{
			None = -1, Vertex = 0, Fragment = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::None;

		while (getline(stream, line))
		{
			if (line.find("#type") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
					type = ShaderType::Vertex;
				else if(line.find("fragment") != std::string::npos)
					type = ShaderType::Fragment;
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		CompileShader(ss[0].str(), ss[1].str());
	}
	OpenGLShader::OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		CompileShader(vertexSrc, fragmentSrc);
	}
	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, (GLint)value);
	}
	void OpenGLShader::UploadUniformInt2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2i(location, (GLint)value.x, (GLint)value.y);
	}
	void OpenGLShader::UploadUniformInt3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3i(location, (GLint)value.x, (GLint)value.y, (GLint)value.z);
	}
	void OpenGLShader::UploadUniformInt4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4i(location, (GLint)value.x, (GLint)value.y, (GLint)value.z, (GLint)value.w);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, (GLfloat)value);
	}
	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, (GLfloat)value.x, (GLfloat)value.y);
	}
	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z);
	}
	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, (GLfloat)value.x, (GLfloat)value.y, (GLfloat)value.z, (GLfloat)value.w);
	}

	void OpenGLShader::UploadUniformMat2(const std::string& name, const glm::mat2& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}
	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::CompileShader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		//	Vertex shader
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* vertexCSrc = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &vertexCSrc, 0);
		glCompileShader(vertexShader);

		GLint isCompiledVS = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiledVS);
		if (isCompiledVS == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(vertexShader);

			KD_CORE_ERROR("{0}", errorLog.data());
			KD_CORE_ASSERT(false, "Failed to compile vertex shader");
		}

		//	Fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* fragmentCSrc = (const GLchar*)fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &fragmentCSrc, 0);
		glCompileShader(fragmentShader);

		GLint isCompiledFS = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiledFS);
		if (isCompiledFS == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &errorLog[0]);

			glDeleteShader(fragmentShader);

			KD_CORE_ERROR("{0}", errorLog.data());
			KD_CORE_ASSERT(false, "Failed to compile fragment shader");
		}

		//	Program
		m_RendererID = glCreateProgram();

		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			KD_CORE_ERROR("{0}", infoLog.data());
			KD_CORE_ASSERT(false, "Failed to link shader program");
		}

		//	Clear shaders
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}
}
