#pragma once

#include "Observer.h"

namespace zr
{
	class Progressable : public Observable<Progressable, float>
	{
	public:
		Progressable() :
			Observable()
		{

		}

		virtual ~Progressable()
		{

		}
	};

	class ProgressListener : public Observer<float>
	{
	public:
		ProgressListener()
		{

		}

		virtual ~ProgressListener()
		{

		}
	};
}
