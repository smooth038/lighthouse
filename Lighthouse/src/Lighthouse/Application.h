#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Events/WindowEvent.h"
#include "LayerStack.h"
#include "Window.h"
#include "ImGuiLayer.h"

namespace Lighthouse {

	class Application
	{
	public:
		Application();
		Application(int width, int height, const char* windowTitle = "Window Title");
		virtual ~Application();

		void run();
		bool onEvent(Event& event);

		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);

		void pushOverlay(std::shared_ptr<Layer> layer);
		void pushLayer(std::shared_ptr<Layer> layer);

		static Application* get() { return s_instance; }
		std::shared_ptr<ImGuiLayer> getImGuiLayer() { return _imGuiLayer; };
		
	protected:
		std::unique_ptr<Window> _window;

	private:
		static Application* s_instance;
		LayerStack _layerStack;
		std::shared_ptr<ImGuiLayer> _imGuiLayer;
		bool _isRunning = true;

		unsigned int _width;
		unsigned int _height;
	};

	// to be defined in client
	std::shared_ptr<Application> createApplication();

}
