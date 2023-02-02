#include <KDEngine.h>

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{}

	void OnUpdate() override
	{
		KD_INFO("Example Layer Update");
	}

	void OnEvent(KDE::Event& e) override
	{
		KD_TRACE("{0}", e);
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