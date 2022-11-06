#include "lhpch.h"

#include "Application.h"
#include "Renderer/Camera.h"
#include "Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Lighthouse
{
	Application::Application() : Application(800, 600) {}

	Application::Application(int width, int height, const char* windowTitle)
		: _width(width), _height(height), _imGuiLayer()
	{
		_window = std::make_unique<Window>(_width, _height, windowTitle);
		_window->setCallback([this](auto& event) { return onEvent(event); });

		Renderer::init(_width, _height);

		_imGuiLayer = std::make_shared<ImGuiLayer>("ImGui Layer", _window);
		pushLayer(_imGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::run()
	{
		while (_isRunning)
		{
			for (auto& layer : _layerStack)
			{
				layer->onUpdate();
			}

			_imGuiLayer->begin();
			for (auto& layer : _layerStack)
			{
				layer->onImGuiRender();
			}
			_imGuiLayer->end();

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

	void Application::pushLayer(std::shared_ptr<Layer> layer)
	{
		layer->onAttach();
		_layerStack.pushLayer(layer);
	}

	

}
