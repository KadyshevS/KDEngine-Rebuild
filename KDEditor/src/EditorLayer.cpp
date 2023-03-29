#include "kdpch.h"
#include "EditorLayer.h"

namespace KDE
{
	EditorLayer::EditorLayer()
		: Layer("KDEditor")
	{}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;
		fbSpec.Width = 1440;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = MakeRef<Scene>();
	//
	//	m_SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
	//	m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));
	//
	//	m_SquareEntityRed = m_ActiveScene->CreateEntity("Red Square Entity");
	//	m_SquareEntityRed.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));
	//
	//	m_CameraEntity = m_ActiveScene->CreateEntity("Camera 1 Entity");
	//	m_CameraEntity.AddComponent<CameraComponent>();
	//
	//	m_CameraEntity2 = m_ActiveScene->CreateEntity("Camera 2 Entity");
	//	m_CameraEntity2.AddComponent<CameraComponent>();
	//	m_CameraEntity2.GetComponent<CameraComponent>().Primary = false;
	//
	//	class CameraController : public ScriptableEntity
	//	{
	//	public:
	//		void OnCreate()
	//		{
	//			KD_TRACE("OnCreateFunc()");
	//		}
	//		void OnDestroy()
	//		{
	//			KD_TRACE("OnDestroyFunc()");
	//		}
	//
	//		void OnUpdate(Timestep ts)
	//		{
	//			auto& position = GetComponent<TransformComponent>().Translation;
	//			float speed = 5.0f;
	//
	//			if (Input::IsKeyPressed(Key::Left))
	//			{
	//				position.x -= speed * ts;
	//			}
	//			else if (Input::IsKeyPressed(Key::Right))
	//			{
	//				position.x += speed * ts;
	//			}
	//
	//			if (Input::IsKeyPressed(Key::Up))
	//			{
	//				position.y += speed * ts;
	//			}
	//			else if (Input::IsKeyPressed(Key::Down))
	//			{
	//				position.y -= speed * ts;
	//			}
	//		}
	//	};
	//	m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
	//	m_CameraEntity2.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ViewportSize.x = (float)spec.Width; m_ViewportSize.y = (float)spec.Height;
		}

	//	Rendering
		m_Framebuffer->Bind();

		Renderer2D::ResetStats();
		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::Clear();

		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}
	void EditorLayer::OnImGuiRender()
	{
	//	Dockspace
		{
			static bool p_open = true;
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
			ImGui::Begin("DockSpace Demo", &p_open, window_flags);
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
					if (ImGui::MenuItem("Exit", "Alt+F4")) Application::Get().Close();

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	
	//	Viewport
		{
			ImGui::Begin("Viewport");

			ImVec2 vpPanelSize = ImGui::GetContentRegionAvail();

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

			uint32_t textureID = m_Framebuffer->GetColorAttachment();
			ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0, 1 }, { 1, 0 });

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

			ImGui::End();
		}
		
	//	Stats
		{
			StatisticsPanel::OnImGuiRender();
		}

	//	Scene Hierarchy
		{
			m_SceneHierarchyPanel.OnImGuiRender();
		}
	}
	void EditorLayer::OnEvent(Event& e) {}
}