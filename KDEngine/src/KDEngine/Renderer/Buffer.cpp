#include "kdpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include <Platform/OpenGL/OpenGLBuffer.h>

namespace KDE
{
	VertexBuffer* VertexBuffer::Create(float* vertices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(unsigned int* indices, unsigned int size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return new OpenGLIndexBuffer(indices, size);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
}