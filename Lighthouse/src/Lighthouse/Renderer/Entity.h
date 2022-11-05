#pragma once

#include "lhpch.h"
#include <glm/glm.hpp>
#include "Shader.h"
#include "Texture.h"

namespace Lighthouse
{

	class Entity
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

		void setTextureSlot(unsigned int index, unsigned int slot);
		void addTextureSlot(unsigned int slot);

		void setObjectIndex(unsigned int index, unsigned int objIndex);
		void addObjectIndex(unsigned int objIndex);

		ShaderType getShaderType(unsigned int index);
		void setShaderType(unsigned int index, ShaderType shaderType);
		void addShaderType(ShaderType shaderType);

		unsigned int getEntityCount();
		void setHiddenState(unsigned int index, bool hidden);
		void addHiddenState(bool hidden);
		bool getHiddenState(unsigned int index);

		glm::mat4 getModelMatrix(unsigned int index);
		void setModelMatrix(unsigned int index, glm::mat4 modelMatrix);
		void addModelMatrix(glm::mat4 modelMatrix);

		std::string getUniqueId() { return _uniqueId; }

		void setHighlightValue(unsigned int index, float highlightValue);
		void addHightlightValue(float highlightValue);

		void render(bool forPicking = false);

	private:
		std::string _uniqueId;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
		std::vector<bool> _hidden;

		std::vector<glm::mat4> _matModels;
		std::vector<unsigned int> _textureSlots;	
		std::vector<unsigned int> _objIndices;
		std::vector<float> _highlightValue;
		std::vector<ShaderType> _shaderType;
	};

}

