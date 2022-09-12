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
		static std::unique_ptr<Shader>& getShader();
		static void setShaderType(ShaderType type);
		static void setShaderModel(glm::mat4 model);
		static void setShaderView(glm::mat4 view);
		static Entity* addEntity(std::string id, std::vector<float> vertices, std::vector<unsigned int> indices, ShaderType shaderType);
		static void renderScene();

		static void setWindowSize(unsigned int width, unsigned int height);
		static void computeProjectionMatrix();

	private:
		// private member variables in .cpp file because of static class
	};

}