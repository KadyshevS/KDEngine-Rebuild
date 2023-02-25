#include <KDEngine/EntryPoint.h>
#include "Sandox2D.h"

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}