#include <kdpch.h>
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace KDE
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		KD_PROFILE_FUNCTION();

		KD_CORE_ASSERT(windowHandle, "Window handle is null.");
	}

	void OpenGLContext::Init()
	{
		KD_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		KD_CORE_ASSERT(status, "Failed to initialize Glad.");

		KD_CORE_INFO("OpenGL Info:\n\t\tVendor: {0}\n\t\tRenderer: {1}\n\t\tVersion: {2}",
			(const char*)glGetString(GL_VENDOR), (const char*)glGetString(GL_RENDERER), (const char*)glGetString(GL_VERSION)
		);

#ifdef KD_ENABLE_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		KD_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "KDEngine requires at least OpenGL version 4.5.");
#endif
	}
	void OpenGLContext::SwapBuffers()
	{
		KD_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}
}