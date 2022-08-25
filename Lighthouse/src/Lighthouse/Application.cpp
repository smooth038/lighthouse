#include "lhpch.h"
#include "Application.h"

#include <GL/glew.h>
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
		GLFWwindow* window;
		glfwInit();
		window = glfwCreateWindow(800, 600, "Window title", NULL, NULL);
		glfwMakeContextCurrent(window);

		while (!glfwWindowShouldClose(window))
		{
			glClearColor(1, 0, 1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}

}
