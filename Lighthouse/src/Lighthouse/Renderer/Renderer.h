#pragma once

#include "../Core.h"
#include "Scene.h"
#include "Shader.h"

namespace Lighthouse
{

	static class LH_API Renderer
	{
	public:
		static void init(unsigned int width, unsigned int height);
		static Entity* addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices);
		static void renderScene();

		static void setWindowSize(unsigned int width, unsigned int height);
		static void computeProjectionMatrix();

	private:
		// private member variables in .cpp file because of static class
	};

}