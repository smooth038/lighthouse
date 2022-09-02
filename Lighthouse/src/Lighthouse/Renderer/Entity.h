#pragma once

#include "lhpch.h"

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

		std::string getUniqueId() { return _uniqueId; }

		void render();

	private:
		std::string _uniqueId;
		std::vector<float> _vertices;
		std::vector<unsigned int> _indices;
	};

}

