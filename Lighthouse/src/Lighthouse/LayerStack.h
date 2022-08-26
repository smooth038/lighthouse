#pragma once

#include "Layer.h"

#include <vector>

namespace Lighthouse
{

	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		
		std::vector<Layer*>::iterator begin() { return _layerStack.begin(); }
		std::vector<Layer*>::iterator end() { return _layerStack.end(); }

	private:
		std::vector<Layer*> _layerStack;
	};

}

