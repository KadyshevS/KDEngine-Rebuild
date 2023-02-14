#include <kdpch.h>
#include <KDEngine.h>

#include <imgui/imgui.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"),
		m_Camera(std::make_shared<KDE::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f))
	{
		//	Drawing
		m_VertexArray.reset(KDE::VertexArray::Create());

		float vertices[6 * 3] =
		{//		 Position				  Color
			-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,
			 0.0f,  0.5f, 0.0f,		0.0f, 1.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		};

		m_VertexBuffer.reset(KDE::VertexBuffer::Create(vertices, sizeof(vertices)));

		{
			KDE::BufferLayout layout =
			{
				{ KDE::ShaderDataType::Float3, "inPos" },
				{ KDE::ShaderDataType::Float3, "inColor" }
			};

			m_VertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(m_VertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		m_IndexBuffer.reset(KDE::IndexBuffer::Create(indices, sizeof(indices)));
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);

		m_VertexArray->Unbind();
		m_VertexBuffer->Unbind();
		m_IndexBuffer->Unbind();

		m_SQVertexArray.reset(KDE::VertexArray::Create());

		float verticesSq[] =
		{
			-1.0f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,
			-1.0f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,
			-0.5f, 1.0f, 0.0f,	0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,	0.0f, 1.0f, 1.0f,
		};

		m_SQVertexBuffer.reset(KDE::VertexBuffer::Create(verticesSq, sizeof(verticesSq)));

		{
			KDE::BufferLayout layout =
			{
				{ KDE::ShaderDataType::Float3, "inPos" },
				{ KDE::ShaderDataType::Float3, "inColor" }
			};

			m_SQVertexBuffer->SetLayout(layout);
		}
		m_SQVertexArray->AddVertexBuffer(m_SQVertexBuffer);

		uint32_t indicesSq[] = { 0, 1, 2,	 2, 1, 3 };
		m_SQIndexBuffer.reset(KDE::IndexBuffer::Create(indicesSq, sizeof(indicesSq)));
		m_SQVertexArray->SetIndexBuffer(m_SQIndexBuffer);

		m_SQVertexArray->Unbind();
		m_SQVertexBuffer->Unbind();
		m_SQIndexBuffer->Unbind();

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 inPos;
			layout(location = 1) in vec3 inColor;

			uniform mat4 u_ViewProjection;

			out vec3 color;
			
			void main()
			{
				gl_Position = u_ViewProjection * vec4(inPos, 1.0f);
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

		m_Shader.reset(new KDE::Shader(vertexSrc, fragmentSrc));
	}
	~ExampleLayer() {}

	void OnAttach() override
	{
		KD_INFO("Creating Example Layer");
	}
	void OnUpdate(KDE::Timestep ts) override
	{
	//	Input 
		if (KDE::Input::IsKeyPressed(KD_KEY_LEFT))
		{
			m_CameraPosition.x -= m_CameraMoveSpeed * ts.GetSeconds();
		}
		if (KDE::Input::IsKeyPressed(KD_KEY_RIGHT))
		{
			m_CameraPosition.x += m_CameraMoveSpeed * ts.GetSeconds();
		}
		if (KDE::Input::IsKeyPressed(KD_KEY_UP))
		{
			m_CameraPosition.y += m_CameraMoveSpeed * ts.GetSeconds();
		}
		if (KDE::Input::IsKeyPressed(KD_KEY_DOWN))
		{
			m_CameraPosition.y -= m_CameraMoveSpeed * ts.GetSeconds();
		}

		if (KDE::Input::IsKeyPressed('A'))
		{
			m_CameraRotation += m_CameraRotateSpeed * ts.GetSeconds();
		}
		if (KDE::Input::IsKeyPressed('D'))
		{
			m_CameraRotation -= m_CameraRotateSpeed * ts.GetSeconds();
		}

	//	Drawing
		KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		KDE::RendererCommand::Clear();

		KDE::Renderer::BeginScene(m_Camera);

		m_Camera->SetPosition(m_CameraPosition);
		m_Camera->SetRotation(m_CameraRotation);

		KDE::Renderer::Submit(m_VertexArray, m_Shader);
		KDE::Renderer::Submit(m_SQVertexArray, m_Shader);

		KDE::Renderer::EndScene();
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Sample title");
		ImGui::Text("Welcome to KDEngine!");
		ImGui::End();
	}

private:
	std::shared_ptr<KDE::Shader> m_Shader;
	std::shared_ptr<KDE::VertexBuffer> m_VertexBuffer;
	std::shared_ptr<KDE::IndexBuffer> m_IndexBuffer;
	std::shared_ptr<KDE::VertexArray> m_VertexArray;

	std::shared_ptr<KDE::VertexBuffer> m_SQVertexBuffer;
	std::shared_ptr<KDE::IndexBuffer> m_SQIndexBuffer;
	std::shared_ptr<KDE::VertexArray> m_SQVertexArray;

	std::shared_ptr<KDE::OrthographicCamera> m_Camera;

	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraRotation = 0.0f;

	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotateSpeed = 180.0f;
};

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}