#pragma once

#include <glm/glm.hpp>

#include "KDEngine/Codes/KeyCodes.h"
#include "KDEngine/Codes/MouseCodes.h"

namespace KDE 
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}