#include "kdpch.h"
#include "Sandox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_CameraController(std::make_shared<KDE::OrthographicCameraController>(1280.0f / 720.0f, true))
{
}

void Sandbox2D::OnAttach()
{
	m_Texture = KDE::Texture2D::Create("assets/textures/default.png");
}
void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(KDE::Timestep ts)
{
//	Profiling
	KD_PROFILE_FUNCTION();

//	Input
	{
		KD_PROFILE_SCOPE("Sandbox2D Camera Updating");
		m_CameraController->OnUpdate(ts);
	}

	{
		KD_PROFILE_SCOPE("Sandbox2D Input Handling");
		if (KDE::Input::IsKeyPressed('W'))
			m_QuadPosition.y += m_QuadMoveSpeed * ts;
		else if (KDE::Input::IsKeyPressed('S'))
			m_QuadPosition.y -= m_QuadMoveSpeed * ts;
		if (KDE::Input::IsKeyPressed('A'))
			m_QuadPosition.x -= m_QuadMoveSpeed * ts;
		else if (KDE::Input::IsKeyPressed('D'))
			m_QuadPosition.x += m_QuadMoveSpeed * ts;

		if (KDE::Input::IsKeyPressed('Q'))
			m_QuadScale += m_QuadScaleSpeed * ts;
		else if (KDE::Input::IsKeyPressed('E'))
			m_QuadScale -= m_QuadScaleSpeed * ts;
	}

//	Drawing
	{
		KD_PROFILE_SCOPE("Sandbox2D Renderer Prep");
		KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		KDE::RendererCommand::Clear();
	}
	{
		KD_PROFILE_SCOPE("Sandbox2D Renderer Update");
		KDE::Renderer2D::BeginScene(m_CameraController->GetCamera());

		KDE::Renderer2D::DrawQuad({ 7.0f, 3.0f, 0.1f }, { 2.0f, 3.5f }, { 0.8f, 0.2f, 0.3f, 1.0f });
		KDE::Renderer2D::DrawQuad({ -7.0f, 2.0f, 0.1f }, { 2.0f, 2.5f }, { 0.2f, 0.8f, 0.3f, 1.0f });
		KDE::Renderer2D::DrawQuad({ 3.0f, 1.0f, 0.1f }, { 2.5f, 3.5f }, { 0.2f, 0.3f, 0.8f, 1.0f });

		KDE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f }, m_Texture, 10.0f);

		KDE::Renderer2D::DrawQuad(m_QuadPosition, m_QuadScale, u_Color);

		KDE::Renderer2D::EndScene();
	}
}
void Sandbox2D::OnImGuiRender()
{
	KD_PROFILE_FUNCTION();

	ImGui::Begin("Sandbox 2D Test");
	ImGui::TextColored({ 0.2f, 0.3f, 0.8f, 1.0f }, "Position");
	ImGui::SliderFloat("X", &m_QuadPosition.x, -10.0f, 10.0f, "%.1f");
	ImGui::SliderFloat("Y", &m_QuadPosition.y, -10.0f, 10.0f, "%.1f");
	ImGui::TextColored({ 0.2f, 0.3f, 0.8f, 1.0f }, "Scale");
	ImGui::SliderFloat("X ", &m_QuadScale.x, 0.5f, 10.0f, "%.1f");
	ImGui::SliderFloat("Y ", &m_QuadScale.y, 0.5f, 10.0f, "%.1f");
	ImGui::TextColored({ 0.2f, 0.3f, 0.8f, 1.0f }, "Color");
	ImGui::ColorEdit3("", glm::value_ptr(u_Color));
	ImGui::End();
}
void Sandbox2D::OnEvent(KDE::Event& e)
{
	m_CameraController->OnEvent(e);
}
