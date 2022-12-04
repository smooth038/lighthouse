#include "lhpch.h"
#include "Scene.h"
#include "Renderer.h"
#include "RenderCommand.h"

#include <GL/glew.h>

namespace Lighthouse
{

    Scene::Scene()
    {
    }

    std::unique_ptr<Entity>& Scene::addEntity(Entity& e)
    {
        std::unique_ptr<Entity> ptr = std::make_unique<Entity>(e);
        _entities.push_back(std::move(ptr));
        _entityIndexByName.insert(std::make_pair<>(e.getUniqueId(), _entities.size() - 1));
        return _entities.back();
    }

    std::unique_ptr<Entity>& Scene::getEntityById(const std::string& id)
    {
		int i = _entityIndexByName[id];
		return _entities[i];
    }

    bool Scene::entityExists(const std::string& id)
    {
        return _entityIndexByName.count(id);
    }

    void Scene::removeEntityById(std::string& id)
    {
        _entities.erase(std::remove_if(_entities.begin(), _entities.end(), [id](std::unique_ptr<Entity>& e) { return e->getUniqueId() == id; }), _entities.end());
        _entityIndexByName.erase(id);
    }

    void Scene::listEntities()
    {
        for (auto& e : _entities)
        {
            std::vector<bool> hiddenEntities;
            for (int i = 0; i < e->getEntityCount(); i++)
            {
                if (e->getHiddenState(i)) hiddenEntities.push_back(i);
            }

            LH_CORE_INFO("There are {0} entities with id {1} positioned at :", e->getEntityCount() - hiddenEntities.size(), e->getUniqueId());
            for (int i = 0; i < e->getEntityCount(); i++)
            {
                if (std::find(hiddenEntities.begin(), hiddenEntities.end(), i) != hiddenEntities.end())
                {
                    continue;
                }
                LH_CORE_INFO("(x, z) = ({0}, {1})", e->getModelMatrix(i)[3].x, e->getModelMatrix(i)[3].z);
            }
        }
    }

    void Scene::clearScene()
    {
        _entities.clear();
        _entityIndexByName.clear();
    }

    void Scene::render(bool forPicking)
    {
        if (forPicking)
        {
            Lighthouse::RenderCommand::clearCanvas(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else
        {
			glBindFramebuffer(GL_FRAMEBUFFER, Renderer::getRenderFbo());
            Lighthouse::RenderCommand::clearCanvas(0.2f, 0.624f, 0.804f, 1.0f);
        }

        for (auto& e : _entities)
        {
            e->render(forPicking);
        }
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

}
