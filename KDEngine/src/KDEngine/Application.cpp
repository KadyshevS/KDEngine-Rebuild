#include <kdpch.h>

#include "Application.h"
#include "Log.h"

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

		glGenVertexArrays(1, &m_VertexArray);
		glBindVertexArray(m_VertexArray);

		glGenBuffers(1, &m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[6 * 3] = 
		{//		 Position				  Color
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)) );

		unsigned int indices[3] = { 0, 1, 2 };

		glGenBuffers(1, &m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inColor;

			out vec3 color;
			
			void main()
			{
				gl_Position = vec4(inPos, 1.0f);
				color = inColor;
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			in vec3 color;

			out vec4 fragColor;
			
			void main()
			{
				fragColor = vec4(color, 1.0f);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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
			glClearColor(0.1, 0.1, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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