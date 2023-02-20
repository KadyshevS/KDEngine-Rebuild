#include "kdpch.h"
#include "WindowsWindow.h"

#include "KDEngine/Log.h"

#include "KDEngine/Events/ApplicationEvent.h"
#include "KDEngine/Events/MouseEvent.h"
#include "KDEngine/Events/KeyEvent.h"

#include <Platform/OpenGL/OpenGLContext.h>

#include <glad/glad.h>

namespace KDE
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		KD_CORE_ERROR("GLFW Error: ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		m_Context->SwapBuffers();
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}
	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		KD_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			KD_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwMakeContextCurrent(m_Window);

	//	Initializing Glad
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		KD_CORE_ASSERT(status, "Failed to initialize Glad.");

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		glfwSetWindowSizeCallback(m_Window, 
			[](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent ev(width, height);
				KD_CORE_TRACE(ev);
				data.EventCallback(ev);
			}
		);

		glfwSetWindowCloseCallback(m_Window, 
			[](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent ev;
				data.EventCallback(ev);
			}
		);

		glfwSetKeyCallback(m_Window,
			[](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent ev(key, 0);
						data.EventCallback(ev);
					}
					break;
					case GLFW_RELEASE:
					{
						KeyReleasedEvent ev(key);
						data.EventCallback(ev);
					}
					break;
					case GLFW_REPEAT:
					{
						KeyPressedEvent ev(key, 1);
						data.EventCallback(ev);
					}
					break;
				}
			}
		);

		glfwSetCharCallback(m_Window,
			[](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent ev(keycode);
				data.EventCallback(ev);
			}
		);
		
		glfwSetMouseButtonCallback(m_Window, 
			[](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent ev(button);
						data.EventCallback(ev);
					}
					break;
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent ev(button);
						data.EventCallback(ev);
					}
					break;
				}
			}
		);

		glfwSetScrollCallback(m_Window,
			[](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent ev((float)xOffset, (float)yOffset);
				data.EventCallback(ev);
			}
		);

		glfwSetCursorPosCallback(m_Window, 
			[](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent ev((float)xPos, (float)yPos);
				data.EventCallback(ev);
			}
		);
	}
	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}
}