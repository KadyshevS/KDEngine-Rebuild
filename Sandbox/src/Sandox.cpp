#include <KDEngine.h>
#include <KDEngine/EntryPoint.h>
#include "Sandox2D.h"

class ExampleLayer : public KDE::Layer
{
public:
	ExampleLayer()
		: Layer("Example Layer")
	{}
	~ExampleLayer() {}
private:
};

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}