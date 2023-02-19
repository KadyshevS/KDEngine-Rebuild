#pragma once

namespace KDE
{
	class KD_API GraphicsContext
	{
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};
}