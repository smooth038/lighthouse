#pragma once

#include "lhpch.h"
#include "Entity.h"

namespace Lighthouse {

	class LH_API Scene
	{
	public:
		Scene();
		~Scene();

		Entity* addEntity(Entity e);
		Entity* getEntityById(std::string& id);
		void removeEntityById(std::string& id);

		void render();

	private:
		std::vector<Entity*> _entities;
	};

}

