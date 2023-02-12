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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuf) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuf) override;

		inline virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override { return m_VertexBuffers; }
		inline virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
	private:
		uint32_t m_RendererID;

		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}