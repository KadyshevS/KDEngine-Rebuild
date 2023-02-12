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

	//	Drawing

		m_VertexArray.reset(VertexArray::Create());

		float vertices[6 * 3] = 
		{//		 Position				  Color
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		};

		m_VertexBuffer.reset( VertexBuffer::Create(vertices, sizeof(vertices)) );

		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "inPos" },
				{ ShaderDataType::Float3, "inColor" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset( IndexBuffer::Create(indices, sizeof(indices)) );
		m_VertexArray->SetIndexBuffer( m_IndexBuffer );
		
		m_VertexArray->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();

		m_SQVertexArray.reset( VertexArray::Create() );

		float verticesSq[] =
		{
			-1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,
			-1.0f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,
			-0.5f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,
		};

		m_SQVertexBuffer.reset(VertexBuffer::Create(verticesSq, sizeof(verticesSq)));

		{
			BufferLayout layout =
			{
				{ ShaderDataType::Float3, "inPos" },
				{ ShaderDataType::Float3, "inColor" }
			};

			m_SQVertexBuffer->SetLayout(layout);
		}
		m_SQVertexArray->AddVertexBuffer(m_SQVertexBuffer);

		uint32_t indicesSq[] = { 0, 1, 2,	 2, 1, 3 };
		m_SQIndexBuffer.reset( IndexBuffer::Create(indicesSq, sizeof(indicesSq)) );
		m_SQVertexArray->SetIndexBuffer(m_SQIndexBuffer);

		m_SQVertexArray->Unbind();
		m_SQVertexBuffer->Unbind();
		m_SQIndexBuffer->Unbind();

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

			Renderer::BeginScene();

			Renderer::Submit(m_VertexArray);
			Renderer::Submit(m_SQVertexArray);

			Renderer::EndScene();

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