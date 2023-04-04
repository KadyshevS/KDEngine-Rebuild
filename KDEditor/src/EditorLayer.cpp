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
		fbSpec.Width = 1600;
		fbSpec.Height = 900;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_ActiveScene = MakeRef<Scene>();

		m_EditorCamera = MakeRef<EditorCamera>();

#if 0
		m_SquareEntity = m_ActiveScene->CreateEntity("Square Entity");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.8f, 1.0f));

		m_SquareEntityRed = m_ActiveScene->CreateEntity("Red Square Entity");
		m_SquareEntityRed.AddComponent<SpriteRendererComponent>(glm::vec4(0.8f, 0.2f, 0.3f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera 1 Entity");
		m_CameraEntity.AddComponent<CameraComponent>();

		m_CameraEntity2 = m_ActiveScene->CreateEntity("Camera 2 Entity");
		m_CameraEntity2.AddComponent<CameraComponent>();
		m_CameraEntity2.GetComponent<CameraComponent>().Primary = false;

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{
				KD_TRACE("OnCreateFunc()");
			}
			void OnDestroy()
			{
				KD_TRACE("OnDestroyFunc()");
			}

			void OnUpdate(Timestep ts)
			{
				auto& position = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::Left))
				{
					position.x -= speed * ts;
				}
				else if (Input::IsKeyPressed(Key::Right))
				{
					position.x += speed * ts;
				}

				if (Input::IsKeyPressed(Key::Up))
				{
					position.y += speed * ts;
				}
				else if (Input::IsKeyPressed(Key::Down))
				{
					position.y -= speed * ts;
				}
			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_CameraEntity2.AddComponent<NativeScriptComponent>().Bind<CameraController>();
#endif

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		SceneSerializer serializer(m_ActiveScene);
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
			m_EditorCamera->SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
			m_ViewportSize.x = (float)spec.Width; m_ViewportSize.y = (float)spec.Height;
		}

	//	Rendering
		m_Framebuffer->Bind();

		Renderer2D::ResetStats();
		RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RendererCommand::Clear();

		if(m_CameraSwitch)
			m_ActiveScene->OnUpdateEditor(ts, *m_EditorCamera);
		else
			m_ActiveScene->OnUpdateRuntime(ts);

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
			ImGuiStyle& style = ImGui::GetStyle();
			float minWinSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = minWinSizeX;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New"))
						NewScene();
					if (ImGui::MenuItem("Open...", "Ctrl+O"))
						LoadScene();
					if (ImGui::MenuItem("Save as...", "Ctrl+Shift+S"))
						SaveSceneAs();
					
					ImGui::Separator();

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
			ImGui::Image((void*)textureID, { m_ViewportSize.x, m_ViewportSize.y }, { 0.0, 1.0 }, { 1.0, 0.0 });

			m_ViewportFocused = ImGui::IsWindowFocused();
			m_ViewportHovered = ImGui::IsWindowHovered();
			Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);

		//	Gizmos
			{
				bool snap = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
				float snapValue = 0.5f;
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					snapValue = 45.0f;

				float snapValues[3] = { snapValue, snapValue, snapValue };

				Entity selectedEnt = m_SceneHierarchyPanel.GetSelectedEntity();
				if (selectedEnt)
				{
					ImGuizmo::SetOrthographic(false);
					ImGuizmo::SetDrawlist();

					float winWidth = (float)ImGui::GetWindowWidth();
					float winHeight = (float)ImGui::GetWindowHeight();
					float winPosX = (float)ImGui::GetWindowPos().x;
					float winPosY = (float)ImGui::GetWindowPos().y;

					ImGuizmo::SetRect(winPosX, winPosY, winWidth, winHeight);

					auto& tc = selectedEnt.GetComponent<TransformComponent>();
					glm::mat4 transform = tc.Transform();

					if (m_CameraSwitch)
					{
						auto& camera = *m_EditorCamera;
						const glm::mat4& camProj = camera.GetProjection();
						glm::mat4 camView = glm::inverse(camera.GetTransform());

						ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
							m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
					}
					else if(m_ActiveScene->GetPrimaryCamera())
					{
						auto camEntity = m_ActiveScene->GetPrimaryCamera();
						const auto& camera = camEntity.GetComponent<CameraComponent>().Camera;
						const glm::mat4& camProj = camera.GetProjection();
						glm::mat4 camView = glm::inverse(camEntity.GetComponent<TransformComponent>().Transform());

						ImGuizmo::Manipulate(glm::value_ptr(camView), glm::value_ptr(camProj),
							m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
					}

					if (ImGuizmo::IsUsing())
					{
						glm::vec3 translation, rotation, scale;
						Math::DecomposeTransform(transform, translation, rotation, scale);

						glm::vec3 deltaRot = rotation - tc.Rotation;
						tc.Translation = translation;
						tc.Rotation += deltaRot;
						tc.Scale = scale;
					}
				}
			}

			ImGui::End();
		}
		
	//	Stats
		{
			ImGui::Begin("Renderer2D Statistics");

			StatisticsPanel::OnImGuiRender();
			ImGui::Separator();

			if (ImGui::Checkbox("Editor / Runtime", &m_CameraSwitch));

			ImGui::End();
		}

	//	Scene Hierarchy
		{
			m_SceneHierarchyPanel.OnImGuiRender();
		}

	}
	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool ctrlPressed = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shiftPressed = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
		//	Shortcuts
			case Key::N:
			{
				if (ctrlPressed)
					NewScene();
			}
			break;

			case Key::O:
			{
				if (ctrlPressed)
					LoadScene();
			}
			break;

			case Key::S:
			{
				if (ctrlPressed && shiftPressed)
					SaveSceneAs();
			}
			break;

		//	Gizmos
			case Key::Q:
			{
				m_GizmoType = ImGuizmo::OPERATION::UNIVERSAL;
			}
			break;
			case Key::E:
			{
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			}
			break;

			case Key::R:
			{
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			}
			break;

			case Key::T:
			{
				m_GizmoType = ImGuizmo::OPERATION::SCALE;
			}
			break;

			default:
				break;
		}

		return false;
	}
	void EditorLayer::OnEvent(Event& e) 
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(KD_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}
	void EditorLayer::NewScene()
	{
		m_ActiveScene = MakeRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}
	void EditorLayer::LoadScene()
	{
		m_ActiveScene = MakeRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		std::string filepath = FileDialogs::OpenFile("KDEngine Scene (*.kds)\0*.kds\0");
		if (!filepath.empty())
		{
			SceneSerializer ser(m_ActiveScene);
			ser.Deserialize(filepath);
		}
	}
	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("KDEngine Scene (*.kds)\0*.kds\0");
		if (!filepath.empty())
		{
			SceneSerializer ser(m_ActiveScene);
			ser.Serialize(filepath.ends_with(".kds") ? filepath : (filepath + ".kds"));
		}
	}
}