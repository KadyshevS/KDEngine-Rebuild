#pragma once

#include <KDEngine/Layer.h>

#include <KDEngine/Events/ApplicationEvent.h>
#include <KDEngine/Events/KeyEvent.h>
#include <KDEngine/Events/MouseEvent.h>

namespace KDE
{
	class KD_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}