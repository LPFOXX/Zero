#pragma once

#include "Layer.h"

namespace zr
{
	class LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();

		void pushLayer(Layer* layer);
		void popLayer(Layer* layer);
		void pushOverlay(Layer* layer);
		void popOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin();
		std::vector<Layer*>::iterator end();
		std::vector<Layer*>::reverse_iterator rbegin();
		std::vector<Layer*>::reverse_iterator rend();

		std::vector<Layer*>::const_iterator begin() const;
		std::vector<Layer*>::const_iterator end() const;
		std::vector<Layer*>::const_reverse_iterator rbegin() const;
		std::vector<Layer*>::const_reverse_iterator rend() const;

	private:
		std::vector<Layer*> mLayers;
		unsigned mLastLayerIndex;
	};
}
