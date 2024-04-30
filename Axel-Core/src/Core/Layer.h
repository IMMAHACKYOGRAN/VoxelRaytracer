#pragma once

#include "Events/Event.h"

namespace Axel
{
	class Layer
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
	};
}