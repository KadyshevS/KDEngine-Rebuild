#pragma once

#include <KDEngine/Renderer/Buffer.h>

namespace KDE
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, unsigned int size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		unsigned int m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(unsigned int* indices, unsigned int size);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		inline virtual unsigned int GetCount() const override { return m_Count; }
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};
}