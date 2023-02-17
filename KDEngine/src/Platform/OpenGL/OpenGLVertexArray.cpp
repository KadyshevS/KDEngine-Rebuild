#include <kdpch.h>
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace KDE
{
	static GLenum BufferDataTypeToOpenGLDataType(const ShaderDataType& type)
	{
		switch (type)
		{
		case ShaderDataType::None:		return NULL;
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Mat2:		return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Boolean:	return GL_BOOL;
		}

		KD_CORE_ASSERT(false, "Unknown ShaderDataType.");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vBuf)
	{
		glBindVertexArray(m_RendererID);
		vBuf->Bind();

		KD_CORE_ASSERT(vBuf->GetLayout().GetElements().size(), "Vertex buffer has not layout.");

		uint32_t index = 0;
		for (const auto& el : vBuf->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, el.GetComponentCount(),
				BufferDataTypeToOpenGLDataType(el.Type),
				el.Normalized ? GL_TRUE : GL_FALSE,
				vBuf->GetLayout().GetStride(),
				(void*)el.Offset
			);
			index++;
		}

		m_VertexBuffers.push_back(vBuf);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& iBuf)
	{
		glBindVertexArray(m_RendererID);
		iBuf->Bind();

		m_IndexBuffer = iBuf;
	}
}