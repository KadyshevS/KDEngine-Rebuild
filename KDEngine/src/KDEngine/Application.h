#pragma once

#include "Core.h"

#include "Window.h"
#include "KDEngine/LayerStack.h"
#include "KDEngine/ImGui/ImGuiLayer.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Renderer/Shader.h"
#include "Renderer/Buffer.h"

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

		unsigned int m_VertexArray;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		LayerStack m_LayerStack;
		bool m_Running = true;

		static Application* s_Instance;
	};

//	Need to be declared in client region
	Application* CreateApplication();
}
