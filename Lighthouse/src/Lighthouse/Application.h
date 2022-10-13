#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "LayerStack.h"
#include "Window.h"

namespace Lighthouse {

	class Application
	{
	public:
		Application();
		Application(int width, int height);
		virtual ~Application() = default;

		void run();
		bool onEvent(Event& event);

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void pushLayer(std::unique_ptr<Layer>& layer);

	protected:
		std::unique_ptr<Window> _window;

	private:
		LayerStack _layerStack;
		bool _isRunning = true;

		unsigned int _width;
		unsigned int _height;
	};

	// to be defined in client
	std::unique_ptr<Application> createApplication();

}
