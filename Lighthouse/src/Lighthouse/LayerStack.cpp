#include "lhpch.h"
#include "LayerStack.h"

namespace Lighthouse
{

	LayerStack::LayerStack() : _insertIndex(0)
	{
	}

	LayerStack::~LayerStack()
	{
		for (auto& layer : _layerStack)
		{
			layer->onDetach();
		}
	}

	void LayerStack::pushOverlay(std::shared_ptr<Layer> layer)
	{
		_layerStack.emplace_back(layer);
	}

	void LayerStack::pushLayer(std::shared_ptr<Layer> layer)
	{
		_layerStack.emplace(begin() + _insertIndex, layer);
		_insertIndex++;
	}

	void LayerStack::popLayer(std::shared_ptr<Layer> layer)
	{
		auto it = std::find(_layerStack.begin(), _layerStack.end(), layer);
		if (it != _layerStack.end())
		{
			layer->onDetach();
			_layerStack.erase(it);
			_insertIndex--;
		}
	}

}
