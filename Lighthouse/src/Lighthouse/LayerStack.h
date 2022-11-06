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

		void pushLayer(std::shared_ptr<Layer> layer);
		void popLayer(std::shared_ptr<Layer> layer);
		
		std::vector<std::shared_ptr<Layer>>::iterator begin() { return _layerStack.begin(); }
		std::vector<std::shared_ptr<Layer>>::iterator end() { return _layerStack.end(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rbegin() { return _layerStack.rbegin(); }
		std::vector<std::shared_ptr<Layer>>::reverse_iterator rend() { return _layerStack.rend(); }

	private:
		std::vector<std::shared_ptr<Layer>> _layerStack;
	};

}

