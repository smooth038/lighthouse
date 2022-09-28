#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Renderer/Renderer.h"

namespace Lighthouse
{

	class Layer
	{
	public:
		Layer(const std::string& debugName = "Layer") : _debugName(debugName) {}
		virtual ~Layer() {}

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event&) {}

	private:
		std::string _debugName;
	};

}
