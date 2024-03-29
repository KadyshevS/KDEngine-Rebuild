#include <kdpch.h>

#include "Application.h"
#include "Log.h"

#include "KDEngine/Renderer/Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace KDE
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		KD_PROFILE_FUNCTION();

		KD_CORE_ASSERT(!s_Instance, "Application already exist.");
		s_Instance = this;

		m_Window = Scope<Window>(Window::Create(WindowProps(name, 1600u, 900u)));
		m_Window->SetEventCallback(KD_BIND_EVENT_FN(Application::OnEvent));
		m_Window->SetVSync(true);

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application()
	{
		KD_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::OnEvent(Event& e)
	{
		KD_PROFILE_FUNCTION();

		EventDispatcher edis(e);
		edis.Dispatch<WindowCloseEvent>(KD_BIND_EVENT_FN(Application::OnWindowClose));
		edis.Dispatch<WindowResizeEvent>(KD_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}

	void Application::PushLayer(Layer* layer)
	{
		KD_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		KD_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
	}
	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		KD_PROFILE_FUNCTION();

		while (m_Running)
		{
			KD_PROFILE_SCOPE("Application Run");

			float time = (float)glfwGetTime();
			Timestep ts = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* l : m_LayerStack)
				{
					KD_PROFILE_SCOPE("LayerStack OnUpdate");
					l->OnUpdate(ts);
				}
			}
			m_ImGuiLayer->Begin();
			for (Layer* l : m_LayerStack)
			{
				KD_PROFILE_SCOPE("ImGui OnUpdate");
				l->OnImGuiRender();
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}