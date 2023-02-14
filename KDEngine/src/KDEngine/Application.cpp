#include <kdpch.h>

#include "Application.h"
#include "Log.h"

#include "KDEngine/Renderer/Renderer.h"
#include <glad/glad.h>

namespace KDE
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		KD_CORE_ASSERT(!s_Instance, "Application already exist.");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>( Window::Create() );
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}
	Application::~Application() {}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher edis(e);
		edis.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
	}
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
	
	void Application::Run()
	{
		while (m_Running)
		{
			for (Layer* l : m_LayerStack)
				l->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* l : m_LayerStack)
				l->OnImGuiRender();
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}
}