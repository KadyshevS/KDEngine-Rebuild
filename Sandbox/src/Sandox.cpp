#include <KDEngine.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"),
		m_CameraController( std::make_shared<KDE::OrthographicCameraController>(1280.0f / 720.0f, true) ),
		m_ShaderLibrary( new KDE::ShaderLibrary() )
	{
		m_SQVertexArray = KDE::VertexArray::Create();

		float verticesSq[] =
		{
			-0.5f, 0.5f, 0.0f,	0.0f, 1.0f,
			-0.5f,-0.5f, 0.0f,	0.0f, 0.0f,
			 0.5f,-0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f, 0.5f, 0.0f,	1.0f, 1.0f,
		};

		m_SQVertexBuffer = KDE::VertexBuffer::Create(verticesSq, sizeof(verticesSq));

		{
			KDE::BufferLayout layout =
			{
				{ KDE::ShaderDataType::Float3, "inPos" },
				{ KDE::ShaderDataType::Float2, "inTexCoord" }
			};

			m_SQVertexBuffer->SetLayout(layout);
		}
		m_SQVertexArray->AddVertexBuffer(m_SQVertexBuffer);

		uint32_t indicesSq[] = { 1, 0, 3,	 2, 1, 3 };
		m_SQIndexBuffer = KDE::IndexBuffer::Create(indicesSq, sizeof(indicesSq));
		m_SQVertexArray->SetIndexBuffer(m_SQIndexBuffer);

		m_SQVertexArray->Unbind();
		m_SQVertexBuffer->Unbind();
		m_SQIndexBuffer->Unbind();

		auto textureShader = m_ShaderLibrary->Load( "assets/shaders/Texture.glsl" );

		m_Texture = KDE::Texture2D::Create("assets/textures/kot.png");
		m_Texture2 = KDE::Texture2D::Create("assets/textures/win.png");

		KD_INFO(m_ShaderLibrary->Get("Texture")->GetName());
	}
	~ExampleLayer() {}

	void OnAttach() override
	{
		KD_INFO("Creating Example Layer");
	}
	void OnUpdate(KDE::Timestep ts) override
	{
		if (KDE::Input::IsKeyPressed('I'))
			m_QuadMat = glm::translate(m_QuadMat, { 0.0f, m_QuadMoveSpeed * ts, 0.0f });
		else if (KDE::Input::IsKeyPressed('K'))
			m_QuadMat = glm::translate(m_QuadMat, { 0.0f, -m_QuadMoveSpeed * ts, 0.0f });
		if (KDE::Input::IsKeyPressed('L'))
			m_QuadMat = glm::translate(m_QuadMat, { m_QuadMoveSpeed * ts, 0.0f, 0.0f });
		else if (KDE::Input::IsKeyPressed('J'))
			m_QuadMat = glm::translate(m_QuadMat, { -m_QuadMoveSpeed * ts, 0.0f, 0.0f });

		if (KDE::Input::IsKeyPressed('O'))
			m_QuadMat = glm::rotate(m_QuadMat, glm::radians(m_QuadRotateSpeed * ts), {0.0f, 0.0f, 1.0f});
		else if (KDE::Input::IsKeyPressed('P'))
			m_QuadMat = glm::rotate(m_QuadMat, glm::radians(-m_QuadRotateSpeed * ts), { 0.0f, 0.0f, 1.0f });

	//	Drawing
		KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		KDE::RendererCommand::Clear();

		m_CameraController->OnUpdate(ts);

		KDE::Renderer::BeginScene( m_CameraController->GetCamera() );

		auto textureShader = m_ShaderLibrary->Get("Texture");
		std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_Texture->Bind();
		KDE::Renderer::Submit(m_SQVertexArray, textureShader, glm::mat4(1.0f));

		m_Texture2->Bind();
		KDE::Renderer::Submit(m_SQVertexArray, textureShader, m_QuadMat);

		KDE::Renderer::EndScene();
	}
	void OnEvent(KDE::Event& e) override
	{
		m_CameraController->OnEvent(e);
	}

private:
	KDE::Ref<KDE::ShaderLibrary> m_ShaderLibrary;

	KDE::Ref<KDE::VertexBuffer> m_SQVertexBuffer;
	KDE::Ref<KDE::IndexBuffer> m_SQIndexBuffer;
	KDE::Ref<KDE::VertexArray> m_SQVertexArray;

	KDE::Ref<KDE::Texture2D> m_Texture;
	KDE::Ref<KDE::Texture2D> m_Texture2;

	KDE::Ref<KDE::OrthographicCameraController> m_CameraController;

	glm::vec3 m_QuadPosition = {0.0f, 0.0f, 0.0f};
	float m_QuadRotation = 0.0f;
	
	glm::mat4 m_QuadMat = glm::mat4(1.0f);

	float m_QuadMoveSpeed = 5.0f;
	float m_QuadRotateSpeed = 180.0f;
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