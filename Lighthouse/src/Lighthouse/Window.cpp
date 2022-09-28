#include "lhpch.h"

#include "Window.h"
#include "Events/WindowEvent.h" 

namespace Lighthouse {

	static uint8_t _windowCount = 0;

	Window::Window(int width, int height, const char* title)
	{
		_data.width = width;
		_data.height = height;
		_data.title = title;

		if (_windowCount == 0)
		{
			int success = glfwInit();
			if (!success) __debugbreak();
		}

		_window = glfwCreateWindow(width, height, title, NULL, NULL);
		_windowCount++;

		glfwMakeContextCurrent(_window);
		glfwSetWindowUserPointer(_window, &_data);

		glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) 
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			windowData->callbackFunc(event);
		});

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
		{
			WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
			WindowResizeEvent event(width, height);
			windowData->callbackFunc(event);
		});
	}

	Window::~Window()
	{
		glfwDestroyWindow(_window);
		_windowCount--;

		if (_windowCount == 0)
		{
			glfwTerminate();
		}
	}

	void Window::repaint()
	{
		glfwSwapBuffers(_window);
		glfwPollEvents();
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(_window);
	}

}
