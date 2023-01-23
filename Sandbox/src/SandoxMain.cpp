#include <KDEngine.h>

class Sandbox : public KDE::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

KDE::Application* KDE::CreateApplication()
{
	return new Sandbox();
}