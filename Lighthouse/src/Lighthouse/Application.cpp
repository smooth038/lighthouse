#include "lhpch.h"

#include "Application.h"
#include "Window.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Lighthouse
{

	Application::Application()
	{
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		Window window(800, 600, "Window title");
		window.setCallback([this](auto& event) { return onEvent(event); });

		Renderer renderer;

		while (_isRunning)
		{
			for (Layer* layer : _layerStack)
			{
				layer->onUpdate(&renderer);
			}

			window.repaint();
		}
	}

	bool Application::onEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.dispatch<WindowCloseEvent>([this](auto& e) { return onWindowClose(e); });
		dispatcher.dispatch<WindowResizeEvent>([this](auto& e) { return onWindowResize(e); });


		return false;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		_isRunning = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		return false;
	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	

}
