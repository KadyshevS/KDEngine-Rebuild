#include <kdpch.h>
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "KDEngine/Log.h"

namespace KDE
{
	Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(name, vertexSrc, fragmentSrc);
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
	Ref<KDE::Shader> Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported."); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLShader>(filePath);
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}


	void ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		KD_CORE_ASSERT(!Exists(name), "Shader already exist.");
		m_Shaders[name] = shader;
	}
	void ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		auto& name = shader->GetName();
		Add(name, shader);
	}
	
	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}
	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		KD_CORE_ASSERT(Exists(name), "Shader not found.");
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}