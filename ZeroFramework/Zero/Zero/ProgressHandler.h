#pragma once

#include "Progressable.h"

namespace zr
{
	using ProgressFunction = std::function<void(float&)>;

	class ProgressHandler
	{
	public:
		ProgressHandler() :
			mProgressFunction(),
			mProgress(0.f)
		{

		}

		virtual ~ProgressHandler() = default;

		virtual void setProgress(float progress)
		{
			mProgress = progress;
		}

		virtual float getProgress() const
		{
			return mProgress;
		}

		virtual void setUpdateFunction(const ProgressFunction& progressFunction)
		{
			mProgressFunction = progressFunction;
		}

	protected:
		ProgressFunction mProgressFunction;
		float mProgress;
	};

	class ModelProgressHandler : public Assimp::ProgressHandler, public ProgressHandler, public Progressable
	{
	public:
		ModelProgressHandler();
		virtual ~ModelProgressHandler();

		// Inherited via ProgressHandler
		virtual bool Update(float percentage = (-1.0F));
	};
}
