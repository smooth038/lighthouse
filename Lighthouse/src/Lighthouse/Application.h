#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "LayerStack.h"

namespace Lighthouse {

	class LH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		bool onEvent(Event& event);

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void pushLayer(Layer* layer);

	private:
		LayerStack _layerStack;
		bool _isRunning = true;
	};

	// to be defined in client
	Application* createApplication();

}
