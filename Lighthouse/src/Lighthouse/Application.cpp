#include "lhpch.h"

#include "Application.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Lighthouse
{
	Application::Application()
		: _width(600), _height(600)
	{
		_window = std::make_unique<Window>(_width, _height, "Window title");
		_window->setCallback([this](auto& event) { return onEvent(event); });

		Renderer::init(_width, _height);
	}

	void Application::run()
	{

		while (_isRunning)
		{
			for (auto& layer : _layerStack)
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


		for (auto it = _layerStack.rbegin(); it != _layerStack.rend(); it++) 
		{
			if (event.handled)
				break;
			(*it)->onEvent(event);
		}

		return false;
	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		LH_CORE_INFO("Window closed. {0}", e.toString());
		_isRunning = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e)
	{
		LH_CORE_INFO("Window resized to {0} x {1}", e.getWidth(), e.getHeight());
		Renderer::setWindowSize(e.getWidth(), e.getHeight());
		Renderer::getCamera().setProjection();
		return false;
	}

	void Application::pushLayer(std::unique_ptr<Layer>& layer)
	{
		layer->onAttach();
		_layerStack.pushLayer(layer);
	}

	

}
