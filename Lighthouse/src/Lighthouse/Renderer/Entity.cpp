#include "lhpch.h"
#include "Entity.h"
#include <GL/glew.h>

#include "Renderer.h"
#include "Camera.h"

namespace Lighthouse {

	Entity::Entity(std::string id)
		: _uniqueId(id), _matModels(), _objIndices(), _shaderType(), _textureSlots(), _highlightValue(), _hidden()
	{
		_hidden.push_back(false);
		_matModels.push_back(glm::mat4(1.0f));
		_textureSlots.push_back(0);
		_objIndices.push_back(0);
		_highlightValue.push_back(1.0f);
		_shaderType.push_back(ShaderType::TEXTURE);
	}

	Entity::~Entity()
	{
	}

	std::vector<float> Entity::getVertices()
	{
		return _vertices;
	}

	void Entity::setVertices(std::vector<float> vertices)
	{
		_vertices = vertices;
	}

	void Entity::addVertices(std::vector<float> newVertices)
	{ 
		_vertices.insert(_vertices.end(), newVertices.begin(), newVertices.end()); 
	}

	std::vector<unsigned int> Entity::getIndices()
	{
		return _indices;
	}

	void Entity::setIndices(std::vector<unsigned int> indices)
	{
		_indices = indices;
	}

	void Entity::addIndices(std::vector<unsigned int> newIndices)
	{ 
		_indices.insert(_indices.end(), newIndices.begin(), newIndices.end()); 
	}

	void Entity::setTextureSlot(unsigned int index, unsigned int slot)
	{
		_textureSlots[index] = slot;
	}

	void Entity::addTextureSlot(unsigned int slot)
	{
		_textureSlots.push_back(slot);
	}

	void Entity::setObjectIndex(unsigned int index, unsigned int objIndex)
	{
		_objIndices[index] = objIndex;
	}

	void Entity::addObjectIndex(unsigned int objIndex)
	{
		_objIndices.push_back(objIndex);
	}

	ShaderType Entity::getShaderType(unsigned int index)
	{
		return _shaderType[index];
	}

	void Entity::setShaderType(unsigned int index, ShaderType shaderType)
	{
		_shaderType[index] = shaderType;
	}

	void Entity::addShaderType(ShaderType shaderType)
	{
		_shaderType.push_back(shaderType);
	}

	unsigned int Entity::getEntityCount()
	{
		return _matModels.size();
	}

	glm::mat4 Entity::getModelMatrix(unsigned int index)
	{
		return _matModels[index];
	}

	void Entity::setModelMatrix(unsigned int index, glm::mat4 modelMatrix)
	{
		_matModels[index] = modelMatrix;
	}

	void Entity::addModelMatrix(glm::mat4 modelMatrix)
	{
		_matModels.push_back(modelMatrix);
	}

	void Entity::addHiddenState(bool hidden)
	{
		_hidden.push_back(hidden);
	}

	void Entity::setHiddenState(unsigned int index, bool hidden)
	{
		_hidden[index] = hidden;
	}

	bool Entity::getHiddenState(unsigned int index)
	{
		return _hidden[index];
	}

	void Entity::setHighlightValue(unsigned int index, float highlightValue)
	{
		_highlightValue[index] = highlightValue;
	}

	void Entity::addHightlightValue(float highlightValue)
	{
		_highlightValue.push_back(highlightValue);
	}

	void Entity::render(bool forPicking)
	{
		float* f = &_vertices[0];
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), f, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

		for (int i = 0; i < _matModels.size(); i++)
		{
			if (_hidden[i])
			{
				continue;
			}
			ShaderType shaderType = forPicking ? ShaderType::PICKING : _shaderType[i];
			Renderer::setShaderType(shaderType);
			switch (shaderType)
			{
			case TEXTURE:
				Renderer::getShader()->setUniform1i("u_texture", _textureSlots[i]);
				Renderer::setLightUniforms();
				break;
			case HIGHLIGHT:
				Renderer::getShader()->setUniform1i("u_texture", _textureSlots[i]);
				Renderer::getShader()->setUniform1f("u_highlightValue", _highlightValue[i]);
				Renderer::setLightUniforms();
				break;
			case PICKING:
				Renderer::getShader()->setUniform1ui("u_objIndex", _objIndices[i]);
			}

			Renderer::setShaderModel(_matModels[i]);
			glDrawElements(GL_TRIANGLES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
		}
	}
}