#pragma once

#include "Core.h"

#include "Window.h"
#include "KDEngine/LayerStack.h"
#include "KDEngine/ImGui/ImGuiLayer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Core/Timestep.h"

namespace KDE
{
	class KD_API Application
	{
		friend class WindowsWindow;

	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;
		bool m_Running = true;
		bool m_Minimized = false;

		static Application* s_Instance;
	};

	//	Need to be declared in client region
	Application* CreateApplication();
}
