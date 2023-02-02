#include <KDEngine.h>

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new KDE::ImGuiLayer());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}