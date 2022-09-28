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

	void LayerStack::pushLayer(std::unique_ptr<Layer>& layer)
	{
		_layerStack.emplace_back(std::move(layer));
	}

	void LayerStack::popLayer(std::unique_ptr<Layer>& layer)
	{
		auto it = std::find(_layerStack.begin(), _layerStack.end(), layer);
		if (it != _layerStack.end())
		{
			layer->onDetach();
			_layerStack.erase(it);
		}
	}

}
