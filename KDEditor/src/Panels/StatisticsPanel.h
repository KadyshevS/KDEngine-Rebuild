#pragma once

#include "KDEngine/Renderer/Renderer2D.h"
#include <imgui/imgui.h>

namespace KDE
{
	class StatisticsPanel
	{
	public:
		static void OnImGuiRender()
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Begin("Renderer Stats");

			ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Draw Calls: %d", stats.DrawCalls);
			ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Quads: %d", stats.QuadCount);
			ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Vertices: %d", stats.GetTotalVertexCount());
			ImGui::TextColored({ 0.2f, 0.8f, 0.3f, 1.0f }, "Indices: %d", stats.GetTotalIndexCount());

			ImGui::End();

			ImGui::Begin("Settings");
		}
	};
}