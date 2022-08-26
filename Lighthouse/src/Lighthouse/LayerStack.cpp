#include "lhpch.h"
#include "LayerStack.h"

namespace Lighthouse
{

	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : _layerStack)
		{
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		_layerStack.emplace_back(layer);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto it = std::find(_layerStack.begin(), _layerStack.end(), layer);
		if (it != _layerStack.end())
		{
			layer->onDetach();
			_layerStack.erase(it);
		}
	}

}
