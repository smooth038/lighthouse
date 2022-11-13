#pragma once

#include "../Core.h"
#include "Scene.h"
#include "Shader.h"
#include "Camera.h"

namespace Lighthouse
{
	class Camera;

	class Renderer
	{
	public:
		static void init(unsigned int width, unsigned int height);
		static void initRenderFrameBuffer();
		static void updateRenderFrameBuffer();
		static void initPickingFrameBuffer();
		static void updatePickingFrameBuffer(bool rerender);
		static unsigned int getRenderFbo();
		static unsigned int getRenderTexture();

		static void onWindowResize(unsigned int width, unsigned int height);

		static std::unique_ptr<Shader>& getShader();
		static void setShaderType(ShaderType type);
		static void setShaderModel(glm::mat4 model);

		static Camera& getCamera();

		static glm::vec3 getLightPosition();
		static void setLightPosition(glm::vec3 lightPosition);
		static void setLightColor(glm::vec4 lightColor);
		static void setLightUniforms();

		static std::unique_ptr<Entity>& addEntity(const std::string id, std::vector<float> vertices, std::vector<unsigned int> indices, ShaderType shaderType);
		static std::unique_ptr<Entity>& loadObjFile(const std::string& filepath, const std::string& name);
		static std::unique_ptr<Entity>& loadLvfFile(const std::string& lvfPath, const std::string& name);

		static Scene& getScene();
		static void renderScene();

		static unsigned int getObjectIndexFromPixel(unsigned int x, unsigned int y);

		static void setWindowSize(unsigned int width, unsigned int height);

	private:
		// private member variables in .cpp file because of static class
	};
}