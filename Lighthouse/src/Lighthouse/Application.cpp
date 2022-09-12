#include "lhpch.h"

#include "Application.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Lighthouse
{

	Application::Application()
		: _width(600), _height(600)
	{
		_window = new Window(_width, _height, "Window title");
		_window->setCallback([this](auto& event) { return onEvent(event); });

		Renderer::init(_width, _height);
	}

	Application::~Application()
	{
		delete _window;
	}

	void Application::run()
	{

		while (_isRunning)
		{
			for (Layer* layer : _layerStack)
			{
				layer->onUpdate();
			}

			_window->repaint();
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
		std::cout << "Window resized to " << e.getWidth() << "x" << e.getHeight() << std::endl;
		Renderer::setWindowSize(e.getWidth(), e.getHeight());
		Renderer::computeProjectionMatrix();
		return false;
	}

	void Application::pushLayer(Layer* layer)
	{
		_layerStack.pushLayer(layer);
		layer->onAttach();
	}

	

}
