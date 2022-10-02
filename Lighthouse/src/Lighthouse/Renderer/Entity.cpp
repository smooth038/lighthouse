#include "lhpch.h"
#include "Entity.h"
#include <GL/glew.h>

#include "Renderer.h"
#include "Camera.h"

namespace Lighthouse {

	Entity::Entity(std::string id)
		: _uniqueId(id), _matModel(glm::mat4(1.0f)), _matView(glm::mat4(1.0f)), _shaderType(ShaderType::FLAT_COLOR), _textureSlot(0)
	{
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

	void Entity::setTextureSlot(unsigned int slot)
	{
		_textureSlot = slot;
	}

	ShaderType Entity::getShaderType()
	{
		return _shaderType;
	}

	void Entity::setShaderType(ShaderType shaderType)
	{
		_shaderType = shaderType;
	}

	glm::mat4 Entity::getModelMatrix()
	{
		return _matModel;
	}

	void Entity::setModelMatrix(glm::mat4 modelMatrix)
	{
		_matModel = modelMatrix;
	}

	void Entity::render()
	{
		float* f = &_vertices[0];
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), f, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
		Renderer::setShaderType(_shaderType);
		Renderer::setShaderModel(_matModel);
		Renderer::getCamera().setProjection();
		if (_shaderType == ShaderType::TEXTURE)
		{
			Renderer::getShader()->setUniform1ui("u_texture", _textureSlot);
			Renderer::setLightUniforms();
		}
		glDrawElements(GL_TRIANGLES, static_cast<int>(_indices.size()), GL_UNSIGNED_INT, nullptr);
	}

}