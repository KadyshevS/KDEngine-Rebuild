#include "kdpch.h"
#include "Sandox2D.h"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox 2D"),
	m_CameraController(KDE::OrthographicCameraController(1280.0f / 720.0f, true)),
	m_ParticleSystem(100000)
{}

void Sandbox2D::OnAttach()
{
	Random::Init();

	m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
	m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
	m_Particle.SizeBegin = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
	m_Particle.LifeTime = 1.0f;
	m_Particle.Velocity = { 0.0f, 0.0f };
	m_Particle.VelocityVariation = { -3.0f, -4.0f };
	m_Particle.Position = { 0.0f, 0.0f };

	m_Sheet = KDE::Texture2D::Create("assets/game/textures/RPGsheet.png");
	m_Kust = KDE::SubTexture2D::CreateFromCoords(m_Sheet, { 4, 3 }, {128, 128});
	m_Tree = KDE::SubTexture2D::CreateFromCoords(m_Sheet, { 2, 1 }, {128, 128}, {1.f, 2.f});

	KDE::FramebufferSpecification fbSpec;
	fbSpec.Width = 1280;
	fbSpec.Height = 720;

	m_Framebuffer = KDE::Framebuffer::Create(fbSpec);
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
		KD_PROFILE_SCOPE("Sandbox2D OnUpdate");

		if (KDE::Input::IsMouseButtonPressed(KD_MOUSE_BUTTON_LEFT) && !ImGui::GetIO().WantCaptureMouse)
		{
			m_Particle.VelocityVariation = { Random::IntDist(-4.0, 4.0), Random::IntDist(-4.0, 4.0) };

			auto [x, y] = KDE::Input::GetMousePosition();
			auto width = KDE::Application::Get().GetWindow().GetWidth();
			auto height = KDE::Application::Get().GetWindow().GetHeight();

			auto bounds = m_CameraController.GetBounds();
			auto pos = m_CameraController.GetCamera().GetPosition();
			x = (x / width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
			y = bounds.GetHeight() * 0.5f - (y / height) * bounds.GetHeight();
			m_Particle.Position = { x + pos.x, y + pos.y };
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.OnUpdate(ts);
		m_CameraController.OnUpdate(ts);
	}

//	Drawing
	{
		KD_PROFILE_SCOPE("Sandbox2D Renderer Prep");

		m_Framebuffer->Bind();
		
		KDE::Renderer2D::ResetStats();
		KDE::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		KDE::RendererCommand::Clear();
	}
	{
		KD_PROFILE_SCOPE("Sandbox2D Renderer Draw");

		KDE::Renderer2D::BeginScene(m_CameraController.GetCamera());

		KDE::Renderer2D::DrawQuad({ 0.0f, 0.0f, 0.0f }, {1.0f, 1.0f}, m_Kust);
		KDE::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.0f }, {1.0f, 2.0f}, m_Tree);
		m_ParticleSystem.OnRender();

		KDE::Renderer2D::EndScene();

		m_Framebuffer->Unbind();
	}
}
void Sandbox2D::OnImGuiRender()
{
	KD_PROFILE_FUNCTION();

//////////////////////////////////////////////////////////////////////////
////	Dockspace	
	static bool* p_open = new bool(true);
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
	
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}
	
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;
	
	if (!opt_padding)
	    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", p_open, window_flags);
	if (!opt_padding)
	    ImGui::PopStyleVar();
	
	if (opt_fullscreen)
	    ImGui::PopStyleVar(2);
	
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
	
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit")) KDE::Application::Get().Close();

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();
//////////////////////////////////////////////////////////////////////////

	ImGui::Begin("Settings");

	ImGui::ColorEdit4("Start Color", glm::value_ptr(m_Particle.ColorBegin));
	ImGui::ColorEdit4("End Color", glm::value_ptr(m_Particle.ColorEnd));
	ImGui::DragFloat("Life Time", &m_Particle.LifeTime, 0.1f, 0.0f, 1000.0f);
	
	uint32_t textureID = m_Framebuffer->GetColorAttachment();
	ImGui::Image((void*)textureID, { 1024.0f, 576.0f }, { 0, 1 }, {1, 0});

	ImGui::End();

	auto stats = KDE::Renderer2D::GetStats();
	ImGui::Begin("Renderer Stats");

	ImGui::TextColored({0.2f, 0.8f, 0.3f, 1.0f}, "Draw Calls: %d", stats.DrawCalls);
	ImGui::TextColored({0.2f, 0.8f, 0.3f, 1.0f}, "Quads: %d", stats.QuadCount);
	ImGui::TextColored({0.2f, 0.8f, 0.3f, 1.0f}, "Vertices: %d", stats.GetTotalVertexCount());
	ImGui::TextColored({0.2f, 0.8f, 0.3f, 1.0f}, "Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}
void Sandbox2D::OnEvent(KDE::Event& e)
{
	m_CameraController.OnEvent(e);
}
