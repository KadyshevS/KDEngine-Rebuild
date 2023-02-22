#include "kdpch.h"
#include "Sandox2D.h"

Sandbox2D::Sandbox2D() 
	: Layer("Sandbox 2D"),
	m_CameraController(std::make_shared<KDE::OrthographicCameraController>(1280.0f / 720.0f, true)),
	m_ShaderLibrary(new KDE::ShaderLibrary())
{}

void Sandbox2D::OnAttach()
{

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

	KDE::Renderer2D::BeginScene(m_CameraController->GetCamera());

	KDE::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, u_Color);

	KDE::Renderer2D::EndScene();
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
