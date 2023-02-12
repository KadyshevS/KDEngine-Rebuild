#pragma once

#include <memory>
#include <KDEngine/Renderer/Buffer.h>

namespace KDE
{
	class VertexArray
	{
	public:
		virtual ~VertexArray() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vBuf) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuf) = 0;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() = 0;
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() = 0;

		static VertexArray* Create();
	};
}