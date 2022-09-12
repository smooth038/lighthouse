#pragma once

#include "lhpch.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

namespace Lighthouse
{

	class LH_API Entity
	{
	public:
		Entity(std::string id);
		~Entity();

		std::vector<float> getVertices();
		void setVertices(std::vector<float> vertices);
		void addVertices(std::vector<float> newVertices);
		std::vector<unsigned int> getIndices();
		void setIndices(std::vector<unsigned int> indices);
		void addIndices(std::vector<unsigned int> newIndices);

		void setTextureSlot(unsigned int slot);

		ShaderType getShaderType();
		void setShaderType(ShaderType shaderType);

		glm::mat4 getModelMatrix();
		void setModelMatrix(glm::mat4 modelMatrix);
		glm::mat4 getViewMatrix();
		void setViewMatrix(glm::mat4 viewMatrix);

		std::string getUniqueId() { return _uniqueId; }

		void render();

	private:
		std::string _uniqueId;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;

		ShaderType _shaderType;

		glm::mat4 _matModel;
		glm::mat4 _matView;

		unsigned int _textureSlot;
	};

}

