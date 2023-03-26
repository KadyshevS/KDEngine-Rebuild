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
	}
}