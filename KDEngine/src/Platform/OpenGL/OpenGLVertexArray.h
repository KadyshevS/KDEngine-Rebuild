#pragma once

#include <KDEngine/Renderer/VertexArray.h>

namespace KDE
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vBuf) override;
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& iBuf) override;

		inline virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() override { return m_VertexBuffers; }
		inline virtual const Ref<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
	};
}