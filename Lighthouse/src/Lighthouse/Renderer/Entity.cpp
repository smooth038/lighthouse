#include "lhpch.h"
#include "Entity.h"
#include "GL/glew.h"

namespace Lighthouse {

	Entity::Entity(std::string id)
		: _uniqueId(id)
	{
	}

	Entity::~Entity()
	{
	}

	void Entity::addVertices(std::vector<float> newVertices)
	{ 
		_vertices.insert(_vertices.end(), newVertices.begin(), newVertices.end()); 
	}

	void Entity::addIndices(std::vector<unsigned int> newIndices)
	{ 
		_indices.insert(_indices.end(), newIndices.begin(), newIndices.end()); 
	}

	void Entity::render()
	{
		float* f = &_vertices[0];
		glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), f, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
	}

}