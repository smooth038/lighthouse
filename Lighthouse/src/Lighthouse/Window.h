#pragma once

#include "GLFW/glfw3.h"
#include "Events/Event.h"

namespace Lighthouse
{

	class Window
	{
	public:
		Window(int width, int height, const char* title);
		~Window();

		void setCallback(std::function<void(Event&)> func) { _data.callbackFunc = func; }

		GLFWwindow* getInstance() { return _window; }

		void repaint();
		bool shouldClose();

	private:
		GLFWwindow* _window;

		struct WindowData
		{
			unsigned int width, height;
			const char* title;
			std::function<void(Event&)> callbackFunc;
		};

		WindowData _data;
	};

}
