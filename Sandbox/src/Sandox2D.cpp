#include "kdpch.h"
#include "Sandox2D.h"

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox 2D"),
	m_CameraController(std::make_shared<KDE::OrthographicCameraController>(1280.0f / 720.0f, true)),
	m_ShaderLibrary(new KDE::ShaderLibrary())
{}

void Sandbox2D::OnAttach()
{
	m_SQVertexArray = KDE::VertexArray::Create();

	float verticesSq[] =
	{
		-0.5f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		 0.5f,-0.5f, 0.0f,
		 0.5f, 0.5f, 0.0f,
	};

	m_SQVertexBuffer = KDE::VertexBuffer::Create(verticesSq, sizeof(verticesSq));

	{
		KDE::BufferLayout layout =
		{
			{ KDE::ShaderDataType::Float3, "inPos" }
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

	auto textureShader = m_ShaderLibrary->Load("assets/shaders/ColorSet.glsl");
}
void Sandbox2D::OnDetach()
{
	
}

void Sandbox2D::OnUpdate(KDE::Timestep ts)
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
		m_QuadMat = glm::rotate(m_QuadMat, glm::radians(m_QuadRotateSpeed * ts), { 0.0f, 0.0f, 1.0f });
	else if (KDE::Input::IsKeyPressed('P'))
		m_QuadMat = glm::rotate(m_QuadMat, glm::radians(-m_QuadRotateSpeed * ts), { 0.0f, 0.0f, 1.0f });

	//	Drawing
	KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	KDE::RendererCommand::Clear();

	m_CameraController->OnUpdate(ts);

	KDE::Renderer::BeginScene(m_CameraController->GetCamera());

	auto textureShader = m_ShaderLibrary->Get("ColorSet");
	std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->Bind();
	std::dynamic_pointer_cast<KDE::OpenGLShader>(textureShader)->UploadUniformFloat3("u_Color", u_Color);

	KDE::Renderer::Submit(m_SQVertexArray, textureShader, m_QuadMat);

	KDE::Renderer::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Sandbox 2D Test");
	ImGui::ColorEdit3("Color", glm::value_ptr(u_Color));
	ImGui::End();
}
void Sandbox2D::OnEvent(KDE::Event& e)
{
	m_CameraController->OnEvent(e);
}
