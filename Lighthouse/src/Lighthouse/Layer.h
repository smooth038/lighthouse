#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Renderer/Renderer.h"
#include "Window.h"

namespace Lighthouse
{

	class Layer
	{
	public:
		Layer(const std::string& debugName, std::unique_ptr<Window>& window) : _debugName(debugName), _window(window) {}
		virtual ~Layer() {}

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onEvent(Event&) {}
		virtual void onImGuiRender() {}

	protected:
		std::unique_ptr<Window>& _window;

	private:
		std::string _debugName;
	};

}
