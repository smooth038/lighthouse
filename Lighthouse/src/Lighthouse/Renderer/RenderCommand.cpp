#include "lhpch.h"
#include "RenderCommand.h"

#include <GL/glew.h>

namespace Lighthouse
{

	void RenderCommand::fillCanvas(float red, float green, float blue, float alpha)
	{
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

}
