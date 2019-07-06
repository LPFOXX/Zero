#include <zr_pch.h>

#include "Layer.h"

namespace zr
{
	Layer::Layer(const std::string& name) :
		mName(name)
	{
	}

	Layer::~Layer()
	{
	}

	inline const std::string& Layer::getName()
	{
		return mName;
	}
}
