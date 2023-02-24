#include "kdpch.h"
#include "Sandox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_Camera( KDE::CreateRef<KDE::OrthographicCameraController>(1280.0f/720.0f, 0.0f) )
{
	m_Texture = KDE::Texture2D::Create("assets/textures/flappybird/bird.png");
	m_BackTexture = KDE::Texture2D::Create("assets/textures/flappybird/background.png");
	m_PipeTexture = KDE::Texture2D::Create("assets/textures/flappybird/pipe.png");

	mt = std::mt19937(rd());
	rng = std::uniform_real_distribution<float>(-0.5f, 0.5f);

	for (uint32_t i = 0; i < 7; i++)
	{
		pipes[i] = { {0.0f + m_DistanceBetweenPipes*i, -1.0f + rng(mt), -0.05f} };
	}
}

void Sandbox2D::OnAttach()
{

}
void Sandbox2D::OnDetach()
{
	
}

void Sandbox2D::OnUpdate(KDE::Timestep ts)
{
//	Profiling
	KD_PROFILE_FUNCTION();

//	Input
	if (!m_GameOver)
	{
		if (KDE::Input::IsKeyPressed(KD_KEY_SPACE))
		{
			m_Position.y += (m_Position.y <= 0.85f) ? m_BirdUpSpeed * ts : 0.0f;
			m_Rotation += (m_Rotation + glm::radians(m_BirdRotSpeed * ts) <= m_BirdRotLimitUp) ? glm::radians(m_BirdRotSpeed * ts) : 0.0f;
		}
		else
		{
			m_Position.y -= (m_Position.y >= -0.85f) ? m_BirdDownSpeed * ts : 0.0f;
			m_Rotation -= (m_Rotation - glm::radians(m_BirdRotSpeed * ts) >= m_BirdRotLimitDown) ? glm::radians(m_BirdRotSpeed * ts) : 0.0f;
		}
	}
	else
	{
		m_Position.y -= (m_Position.y >= -0.85f) ? m_BirdDownSpeed * ts : 0.0f;
		m_Rotation -= (m_Rotation - glm::radians(m_BirdRotSpeed * ts) >= m_BirdRotLimitDown) ? glm::radians(m_BirdRotSpeed * ts) : 0.0f;
	}

//	Updating
	for (auto& p : pipes)
	{
		if (
			m_Position.y + (m_Texture->GetHeight() / 2.0f) < p.position.y + m_PipeTexture->GetHeight()/2.0f &&
			m_Position.x + (m_Texture->GetWidth() / 2.0f) > p.position.x + m_PipeTexture->GetWidth()/2.0f 
		)
		{
			m_GameOver = true;
		}
	}

	if (!m_GameOver)
	{
		for (auto& p : pipes)
		{
			p.position.x -= m_PipesSpeed * ts;
			if (p.position.x < -m_DistanceBetweenPipes * 3.0f)
			{
				p.position.x += m_DistanceBetweenPipes * 7.0f;
				p.position.y = -1.0f + rng(mt);
			}
		}
	}

	if(!m_GameOver)
		m_Camera->OnUpdate(ts);

//	Drawing
	KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
	KDE::RendererCommand::Clear();

	KDE::Renderer2D::BeginScene(m_Camera->GetCamera());

	KDE::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 3.7f, 2.0f }, 0.0f, m_BackTexture);	//	Background

	for (auto& p : pipes)
	{
		KDE::Renderer2D::DrawQuad(p.position, { 0.25f, 1.5f }, 0.0f, m_PipeTexture);
		KDE::Renderer2D::DrawQuad({ p.position.x, p.position.y + m_PipesGap, -0.05f }, {0.25f, 1.5f}, glm::radians(180.0f), m_PipeTexture);
	}
	KDE::Renderer2D::DrawQuad(m_Position, {0.25f, 0.25f}, m_Rotation, m_Texture);	//	Bird

	KDE::Renderer2D::EndScene();
}
void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Position");
	ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 1.0f }, " {%.3f, %.3f}", m_Position.x, m_Position.y);
	ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Rotation");
	ImGui::TextColored({ 1.0f, 1.0f, 1.0f, 1.0f }, " %.3f", m_Rotation);
	ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Pipes Gap");
	ImGui::SliderFloat(" ", &m_PipesGap, 1.5f, 2.5f, "%.1f");
	ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Pipes Speed");
	ImGui::SliderFloat("  ", &m_PipesSpeed, 0.3f, 2.0f, "%.1f");
	
	ImGui::End();
}
void Sandbox2D::OnEvent(KDE::Event& e)
{
	m_Camera->OnEvent(e);
}
