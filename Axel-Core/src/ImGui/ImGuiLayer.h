#pragma once
#include "Core/Layer.h"

namespace Axel
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void BlockImGuiLayerEvents(bool block) { m_BlockImGuiEvents = block; }

		void Begin();
		void End();
	private:
		bool m_BlockImGuiEvents = false;
	};
}