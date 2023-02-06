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
		glm::vec3 testVec(12.0f, 45.0f, 3.0f);
		glm::vec3 testVec2(2.0f, 4.0f, 1.0f);
		testVec *= testVec2;
		KD_CORE_TRACE("Created vector 3 ({0}, {1}, {2})", testVec.x, testVec.y, testVec.z);
	}
	void OnUpdate() override
	{
		if (KDE::Input::IsKeyPressed(KD_KEY_RIGHT_SHIFT))
		{
			KD_TRACE("TAB key is pressed.");
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