#pragma once

#include "../Core.h"
#include "Scene.h"

namespace Lighthouse
{

	static class LH_API Renderer
	{
	public:
		static void init();
		static Entity* addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices);
		static void renderScene();

	private:
	};

}