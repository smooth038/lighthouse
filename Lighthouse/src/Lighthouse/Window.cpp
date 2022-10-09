#include "lhpch.h"

#include "Window.h"
#include "Events/WindowEvent.h" 
#include "Events/KeyEvent.h" 
#include "Events/MouseEvent.h"

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

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key);
						data.callbackFunc(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.callbackFunc(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						data.callbackFunc(event);
						break;
					}
				}
			});

		glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int keyCode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keyCode);
				data.callbackFunc(event);
			});

		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.callbackFunc(event);
					break;
				}
				case GLFW_RELEASE:
					MouseButtonReleasedEvent event(button);
					data.callbackFunc(event);
					break;
				}
			});
		
		glfwSetScrollCallback(_window, [](GLFWwindow* window, double offsetX, double offsetY)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event(static_cast<float>(offsetX), static_cast<float>(offsetY));
				data.callbackFunc(event);
			});

		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double mouseX, double mouseY)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event(static_cast<float>(mouseX), static_cast<float>(mouseY));
				data.callbackFunc(event);
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

	void Window::centerMouseCursor()
	{
		glfwSetCursorPos(_window, static_cast<double>((_data.width / 2)), static_cast<double>((_data.height / 2)));
	}

	void Window::hideCursor()
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}

	void Window::unhideCursor()
	{
		glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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
