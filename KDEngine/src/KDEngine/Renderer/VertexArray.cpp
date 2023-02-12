#include <kdpch.h>
#include "VertexArray.h"

#include <KDEngine/Renderer/Renderer.h>
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace KDE
{
	VertexArray* VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexArray();
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
}