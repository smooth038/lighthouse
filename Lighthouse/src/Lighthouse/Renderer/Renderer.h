#pragma once

#include "../Core.h"
#include "Scene.h"
#include "Shader.h"

namespace Lighthouse
{

	class Renderer
	{
	public:
		static void init(unsigned int width, unsigned int height);
		static std::unique_ptr<Shader>& getShader();
		static void setShaderType(ShaderType type);
		static void setShaderModel(glm::mat4 model);
		static void setShaderView(glm::mat4 view);
		static glm::vec3 getLightPosition();
		static void setLightPosition(glm::vec3 lightPosition);
		static void setLightColor(glm::vec4 lightColor);
		static void setLightUniforms();
		static std::unique_ptr<Entity>& addEntity(const std::string id, std::vector<float> vertices, std::vector<unsigned int> indices, ShaderType shaderType);
		static std::unique_ptr<Entity>& loadObjFile(const std::string& filepath, const std::string& name);
		static void renderScene();
		static Scene& getScene();

		static void setWindowSize(unsigned int width, unsigned int height);
		static void computeProjectionMatrix();

	private:
		// private member variables in .cpp file because of static class
	};

}