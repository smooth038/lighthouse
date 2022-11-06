#include "lhpch.h"
#include "LayerStack.h"

namespace Lighthouse
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : _layerStack)
		{
			layer->onDetach();
		}
	}

	void LayerStack::pushLayer(std::shared_ptr<Layer> layer)
	{
		_layerStack.push_back(layer);
	}

	void LayerStack::popLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(_layerStack.begin(), _layerStack.end(), layer);
		if (it != _layerStack.end())
		{
			layer->onDetach();
			_layerStack.erase(it);
		}
	}

}
