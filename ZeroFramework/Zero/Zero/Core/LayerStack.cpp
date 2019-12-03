#include <zr_pch.h>

#include "LayerStack.h"

namespace zr
{
	LayerStack::LayerStack() :
		mLayers(),
		mLastLayerIndex(0U)
	{

	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : mLayers) {
			layer->onDetach();
			delete layer;
		}
	}

	void LayerStack::pushLayer(Layer* layer)
	{
		mLayers.emplace(mLayers.begin() + mLastLayerIndex, layer);
		mLastLayerIndex++;
	}

	void LayerStack::popLayer(Layer* layer)
	{
		auto found = std::find(mLayers.begin(), mLayers.end(), layer);
		if (found != mLayers.begin() + mLastLayerIndex) {
			layer->onDetach();
			mLayers.erase(found);
			mLastLayerIndex--;
		}
	}

	void LayerStack::pushOverlay(Layer* layer)
	{
		mLayers.emplace_back(layer);
	}

	void LayerStack::popOverlay(Layer* layer)
	{
		auto found = std::find(mLayers.begin(), mLayers.end(), layer);
		if (found != mLayers.end()) {
			layer->onDetach();
			mLayers.erase(found);
		}
	}

	std::vector<Layer*>::iterator LayerStack::begin()
	{
		return mLayers.begin();
	}

	std::vector<Layer*>::iterator LayerStack::end()
	{
		return mLayers.end();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rbegin()
	{
		return mLayers.rbegin();
	}

	std::vector<Layer*>::reverse_iterator LayerStack::rend()
	{
		return mLayers.rend();
	}

	std::vector<Layer*>::const_iterator LayerStack::begin() const
	{
		return mLayers.cbegin();
	}

	std::vector<Layer*>::const_iterator LayerStack::end() const
	{
		return mLayers.cend();
	}

	std::vector<Layer*>::const_reverse_iterator LayerStack::rbegin() const
	{
		return mLayers.crbegin();
	}

	std::vector<Layer*>::const_reverse_iterator LayerStack::rend() const
	{
		return mLayers.crend();
	}
}
