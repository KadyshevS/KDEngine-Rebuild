#include <KDEngine/EntryPoint.h>
#include "EditorLayer.h"

namespace KDE
{
	class KDEditor : public Application
	{
	public:
		KDEditor()
			: Application("KDEditor")
		{
			PushLayer(new EditorLayer());
		}
		~KDEditor() {}
	};

	Application* CreateApplication()
	{
		return new KDEditor();
	}
}