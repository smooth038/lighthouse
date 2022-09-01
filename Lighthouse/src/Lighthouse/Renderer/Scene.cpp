#include "lhpch.h"
#include "Scene.h"

namespace Lighthouse
{

    Scene::Scene()
    {
    }

    Scene::~Scene()
    {
    }

    void Scene::addEntity(Entity e)
    {
        _entities.push_back(e);
    }

    Entity& Scene::getEntityById(std::string& id)
    {
        auto match = std::find_if(_entities.begin(), _entities.end(), [id](Entity& e) { return e.getUniqueId() == id; });
        if (match == _entities.end()) throw "Entity id not found!";
        return *match;
    }

    void Scene::removeEntityById(std::string& id)
    {
        _entities.erase(std::remove_if(_entities.begin(), _entities.end(), [id](Entity& e) { return e.getUniqueId() == id; }), _entities.end());
    }

    void Scene::render()
    {
        for (Entity e : _entities)
        {
            e.render();
        }
    }

}
