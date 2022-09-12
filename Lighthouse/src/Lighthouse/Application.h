#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "LayerStack.h"
#include "Window.h"

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
		Window* _window;
		LayerStack _layerStack;
		bool _isRunning = true;

		unsigned int _width;
		unsigned int _height;
	};

	// to be defined in client
	Application* createApplication();

}
