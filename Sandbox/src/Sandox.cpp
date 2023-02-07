#include <KDEngine.h>
#include <imgui/imgui.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Input Layer")
	{}
	~ExampleLayer() {}
	
	void OnAttach() override
	{
		KD_INFO("Creating Example Layer");
	}
	void OnUpdate() override
	{
		if (KDE::Input::IsKeyPressed(KD_KEY_RIGHT_SHIFT))
		{
			KD_TRACE("Right shift key is pressed.");
		}
	}
	void OnImGuiRender() override
	{
		ImGui::Begin("Sample title");
		ImGui::Text("Welcome to KDEngine!");
		ImGui::End();
	}
};

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}