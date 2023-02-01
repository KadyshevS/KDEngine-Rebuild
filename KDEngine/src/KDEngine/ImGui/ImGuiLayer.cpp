#include <kdpch.h>
#include "ImGuiLayer.h"

#include "imgui.h"

namespace KDE
{
	ImGuiLayer::ImGuiLayer()
		: Layer("ImGui Layer")
	{}
	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach()
	{
	}
	void ImGuiLayer::OnDetach()
	{
	}
	void ImGuiLayer::OnUpdate()
	{
	}
	void ImGuiLayer::OnEvent(Event& e)
	{
	}
}