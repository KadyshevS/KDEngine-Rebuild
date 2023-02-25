#include "kdpch.h"
#include "Buffer.h"

#include "Renderer.h"
#include <Platform/OpenGL/OpenGLBuffer.h>

namespace KDE
{
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(size);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLVertexBuffer>(vertices, size);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			KD_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
			return nullptr;
			break;
		case RendererAPI::API::OpenGL:
			return CreateRef<OpenGLIndexBuffer>(indices, size);
			break;
		}

		KD_CORE_ASSERT(false, "Unknown Renderer API.");
		return nullptr;
	}
}