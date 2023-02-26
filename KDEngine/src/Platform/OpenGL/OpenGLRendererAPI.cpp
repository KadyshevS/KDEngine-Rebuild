#include <kdpch.h>
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace KDE
{
	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         KD_CORE_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       KD_CORE_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          KD_CORE_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: KD_CORE_TRACE(message); return;
		}

		KD_CORE_ASSERT(false, "Unknown severity level.");
	}

	void OpenGLRendererAPI::Init()
	{
		KD_PROFILE_FUNCTION();

#ifdef KD_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

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

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vArr, uint32_t indexCount)
	{
		KD_PROFILE_FUNCTION();

		uint32_t count = indexCount ? indexCount : vArr->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}