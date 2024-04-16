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
		virtual void OnUpdate(float ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& e) {}
	};
}