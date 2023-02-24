#include <kdpch.h>
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace KDE
{
	void OpenGLRendererAPI::Init()
	{
		KD_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}
	void OpenGLRendererAPI::SetViewport(int x, int y, int width, int height)
	{
		KD_PROFILE_FUNCTION();

		glViewport(x, y, width, height);
	}
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		KD_PROFILE_FUNCTION();

		glClearColor(color.r, color.g, color.b, color.a);
	}
	void OpenGLRendererAPI::Clear()
	{
		KD_PROFILE_FUNCTION();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vArr)
	{
		KD_PROFILE_FUNCTION();

		glDrawElements(GL_TRIANGLES, vArr->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}