#include <KDEngine.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer"),
		m_Camera(std::make_shared<KDE::OrthographicCamera>(-1.6f, 1.6f, -0.9f, 0.9f)),
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
	//	Input 
		if (KDE::Input::IsKeyPressed(KD_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts.GetSeconds();
		else if (KDE::Input::IsKeyPressed(KD_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts.GetSeconds();
		if (KDE::Input::IsKeyPressed(KD_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts.GetSeconds();
		else if (KDE::Input::IsKeyPressed(KD_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts.GetSeconds();

		if (KDE::Input::IsKeyPressed('I'))
			m_QuadMat = glm::translate(m_QuadMat, { 0.0f, m_CameraMoveSpeed * ts, 0.0f });
		else if (KDE::Input::IsKeyPressed('K'))
			m_QuadMat = glm::translate(m_QuadMat, { 0.0f, -m_CameraMoveSpeed * ts, 0.0f });
		if (KDE::Input::IsKeyPressed('L'))
			m_QuadMat = glm::translate(m_QuadMat, { m_CameraMoveSpeed * ts, 0.0f, 0.0f });
		else if (KDE::Input::IsKeyPressed('J'))
			m_QuadMat = glm::translate(m_QuadMat, { -m_CameraMoveSpeed * ts, 0.0f, 0.0f });

		if (KDE::Input::IsKeyPressed('A'))
			m_CameraRotation += m_CameraRotateSpeed * ts.GetSeconds();
		else if (KDE::Input::IsKeyPressed('D'))
			m_CameraRotation -= m_CameraRotateSpeed * ts.GetSeconds();

		if (KDE::Input::IsKeyPressed('O'))
			m_QuadMat = glm::rotate(m_QuadMat, glm::radians(m_CameraRotateSpeed * ts), {0.0f, 0.0f, 1.0f});
		else if (KDE::Input::IsKeyPressed('P'))
			m_QuadMat = glm::rotate(m_QuadMat, glm::radians(-m_CameraRotateSpeed * ts), { 0.0f, 0.0f, 1.0f });

	//	Drawing
		KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		KDE::RendererCommand::Clear();

		KDE::Renderer::BeginScene(m_Camera);

		auto textureShader = m_ShaderLibrary->Get("Texture");
		std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);

		m_Camera->SetPosition(m_CameraPosition);
		m_Camera->SetRotation(m_CameraRotation);

		m_Texture->Bind();
		KDE::Renderer::Submit(m_SQVertexArray, textureShader, glm::mat4(1.0f));

		m_Texture2->Bind();
		KDE::Renderer::Submit(m_SQVertexArray, textureShader, m_QuadMat);

		KDE::Renderer::EndScene();
	}

private:
	KDE::Ref<KDE::ShaderLibrary> m_ShaderLibrary;

	KDE::Ref<KDE::VertexBuffer> m_SQVertexBuffer;
	KDE::Ref<KDE::IndexBuffer> m_SQIndexBuffer;
	KDE::Ref<KDE::VertexArray> m_SQVertexArray;

	KDE::Ref<KDE::Texture2D> m_Texture;
	KDE::Ref<KDE::Texture2D> m_Texture2;

	KDE::Ref<KDE::OrthographicCamera> m_Camera;

	glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
	float m_CameraRotation = 0.0f;

	glm::mat4 m_QuadMat = glm::mat4(1.0f);

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