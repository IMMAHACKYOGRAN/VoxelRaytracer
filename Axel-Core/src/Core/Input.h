#pragma once

#include "Core/KeyCodes.h"

#include <glm.hpp>

namespace Axel
{
	class Input
	{
	public:
		static bool IsKeyDown(KeyCode key);

		static bool IsMouseButtonDown(MouseButton button);
		static glm::vec2 GetMousePosition();

		static void SetCursorMode(CursorMode mode);
	};
}