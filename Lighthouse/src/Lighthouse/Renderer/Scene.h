#pragma once

#include "lhpch.h"
#include "Entity.h"
#include "Shader.h"

namespace Lighthouse {

	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		std::unique_ptr<Entity>& addEntity(Entity& e);
		std::unique_ptr<Entity>& getEntityById(const std::string& id);
		bool entityExists(const std::string& id);
		void removeEntityById(std::string& id);

		void listEntities();

		void render(bool forPicking = false);

	private:
		std::unordered_map<std::string, int> _entityIndexByName;
		std::vector<std::unique_ptr<Entity>> _entities;
	};

}

