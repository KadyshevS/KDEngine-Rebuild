#include "kdpch.h"
#include "Framebuffer.h"

#include "KDEngine/Core.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace KDE
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return MakeRef<OpenGLFramebuffer>(spec);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer Error.");
		return nullptr;
	}
}