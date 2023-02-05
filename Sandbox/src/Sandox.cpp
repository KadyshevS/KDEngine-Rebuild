#include <KDEngine.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Input Layer")
	{
		glm::vec3 testVec(12.0f, 45.0f, 3.0f);
		glm::vec3 testVec2(2.0f, 4.0f, 1.0f);
		testVec *= testVec2;
		KD_CORE_TRACE("Created vector 3 ({0}, {1}, {2})", testVec.x, testVec.y, testVec.z);
	}
	~ExampleLayer() {}

	void OnUpdate() override
	{
		if (KDE::Input::IsKeyPressed(KD_KEY_RIGHT_SHIFT))
		{
			KD_TRACE("TAB key is pressed.");
		}
	}
};

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new KDE::ImGuiLayer());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}