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
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}