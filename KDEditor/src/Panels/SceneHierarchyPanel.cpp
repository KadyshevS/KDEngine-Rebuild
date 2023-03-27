#include "kdpch.h"
#include "SceneHierarchyPanel.h"

#include "KDEngine/Scene/Components.h"

#include <imgui/imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace KDE
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		KD_CORE_ASSERT(m_Context.get(), "Scene Hierarchy\'s context is not set.");

		ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each(
			[&](auto entityID)
			{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
			}
		);
		
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionEntity = {};

		ImGui::End();

		ImGui::Begin("Properties");

		if (m_SelectionEntity)
		{
			DrawComponents(m_SelectionEntity);
		}

		ImGui::End();
	}
	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionEntity = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flagsEx = ((m_SelectionEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
			bool openedEx = ImGui::TreeNodeEx((void*)((uint64_t)(uint32_t)entity + 1000), flagsEx, tag.c_str());
			ImGui::TreePop();
			if (openedEx)
				ImGui::TreePop();
		}
	}
	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buf[256];
			memset(buf, 0, sizeof(buf));
			strcpy_s(buf, tag.c_str());
			if (ImGui::InputText("Tag", buf, sizeof(buf)))
			{
				tag = std::string(buf);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if ( ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform") )
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.03f);

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<CameraComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera"))
			{
				auto& camera = entity.GetComponent<CameraComponent>().Camera;

				bool& isPrimary = entity.GetComponent<CameraComponent>().Primary;
				ImGui::Checkbox("Primary", &isPrimary);

				const char* projStrs[] = { "Perspective", "Orthographic" };
				const char* currProjStr = projStrs[(int)camera.GetProjectionType()];

				if (ImGui::BeginCombo("Projection", currProjStr))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currProjStr = projStrs[i];
						if (ImGui::Selectable(projStrs[i], isSelected))
						{
							currProjStr = projStrs[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspFOV = camera.GetPerspFOV();
					if (ImGui::DragFloat("FOV", &perspFOV, 0.05f))
						camera.SetPerspFOV(perspFOV);

					float perspNear = camera.GetPerspNearClip();
					if (ImGui::DragFloat("Near", &perspNear, 0.05f))
						camera.SetPerspNearClip(perspNear);

					float perspFar = camera.GetPerspFarClip();
					if (ImGui::DragFloat("Far", &perspFar, 0.05f))
						camera.SetPerspFarClip(perspFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthoSize();
					if (ImGui::DragFloat("Size", &orthoSize, 0.05f))
						camera.SetOrthoSize(orthoSize);

					float orthoNear = camera.GetOrthoNearClip();
					if (ImGui::DragFloat("Near", &orthoNear, 0.05f))
						camera.SetOrthoNearClip(orthoNear);

					float orthoFar = camera.GetOrthoFarClip();
					if (ImGui::DragFloat("Far", &orthoFar, 0.05f))
						camera.SetOrthoFarClip(orthoFar);
				}

				ImGui::TreePop();
			}
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Sprite Renderer"))
			{
				auto& renderer = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(renderer.Color));

				ImGui::TreePop();
			}
		}
	}
}