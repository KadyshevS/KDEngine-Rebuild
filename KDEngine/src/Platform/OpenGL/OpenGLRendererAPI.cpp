#include <kdpch.h>
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace KDE
{
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vArr)
	{
		glDrawElements(GL_TRIANGLES, vArr->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}