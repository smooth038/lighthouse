#pragma once

#include "lhpch.h"
#include "Lighthouse/Layer.h"
#include "Lighthouse/Events/Event.h"

namespace Lighthouse
{
	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(const std::string& debugName, std::unique_ptr<Window>& window) : Layer(debugName, window) {}
		virtual ~ImGuiLayer() = default;

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate() override;
		virtual void onEvent(Event&) override;

		void begin();
		void end();

		void blockEvents(bool block) { _blockEvents = block; }

	private:
		bool _blockEvents = true;
	};

}
