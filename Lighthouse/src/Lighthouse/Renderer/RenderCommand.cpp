#include "lhpch.h"
#include "RenderCommand.h"

#include <GL/glew.h>

namespace Lighthouse
{

	void RenderCommand::fillCanvas(float red, float green, float blue, float alpha)
	{
		glClearColor(red, green, blue, alpha);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}
