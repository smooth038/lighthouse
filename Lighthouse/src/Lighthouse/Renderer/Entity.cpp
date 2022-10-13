#include "lhpch.h"
#include "Entity.h"
#include <GL/glew.h>

#include "Renderer.h"
#include "Camera.h"

namespace Lighthouse {

	Entity::Entity(std::string id)
		: _uniqueId(id), _matModels(), _shaderType(ShaderType::FLAT_COLOR), _textureSlots()
	{
		_matModels.push_back(glm::mat4(1.0f));
		_textureSlots.push_back(0);
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

	ShaderType Entity::getShaderType()
	{
		return _shaderType;
	}

	void Entity::setShaderType(ShaderType shaderType)
	{
		_shaderType = shaderType;
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

	void Entity::render()
	{
		float* f = &_vertices[0];
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), f, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
		Renderer::setShaderType(_shaderType);

		for (int i = 0; i < _matModels.size(); i++)
		{
			if (_shaderType == ShaderType::TEXTURE)
			{
				Renderer::getShader()->setUniform1i("u_texture", _textureSlots[i]);
				Renderer::setLightUniforms();
			}
			Renderer::setShaderModel(_matModels[i]);
			glDrawElements(GL_TRIANGLES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
		}
	}

}