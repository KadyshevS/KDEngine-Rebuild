#include <kdpch.h>
#include "Shader.h"

#include <glad/glad.h>
#include <KDEngine/Log.h>

namespace KDE
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
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
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}