#include <kdpch.h>

#include "Application.h"
#include "Log.h"

#include <glad/glad.h>

namespace KDE
{
	GLenum BufferDataTypeToOpenGLDataType(const ShaderDataType& type)
	{
		switch (type)
		{
			case ShaderDataType::None:		return NULL;
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Mat2:		return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Boolean:	return GL_BOOL;
		}

		KD_CORE_ASSERT(false, "Unknown ShaderDataType.");
		return 0;
	}

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

		float vertices[6 * 3] = 
		{//		 Position				  Color
			-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,
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

		uint32_t index = 0;
		for (const auto& el : m_VertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(
				index, el.GetComponentCount(),
				BufferDataTypeToOpenGLDataType(el.Type),
				el.Normalized ? GL_TRUE : GL_FALSE,
				m_VertexBuffer->GetLayout().GetStride(),
				(void*)el.Offset
			);
			index++;
		}

		unsigned int indices[3] = { 0, 1, 2 };

		m_IndexBuffer.reset( IndexBuffer::Create(indices, sizeof(indices)) );

		glBindVertexArray(0);
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();

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
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

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