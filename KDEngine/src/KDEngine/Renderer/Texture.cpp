#include <kdpch.h>
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace KDE
{
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(width, height);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
}