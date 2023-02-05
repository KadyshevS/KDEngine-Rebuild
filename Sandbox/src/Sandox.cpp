#include <KDEngine.h>

class InputLayer : public KDE::Layer
{
public:
	InputLayer()
		: Layer("Input Layer")
	{}
	~InputLayer() {}

	void OnUpdate() override
	{
		auto [x, y] = KDE::Input::GetMousePosition();
		KD_TRACE("({0}, {1})", x, y);
	}
};

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new InputLayer());
		PushLayer(new KDE::ImGuiLayer());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}