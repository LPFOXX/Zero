#pragma once

#include <assimp/ProgressHandler.hpp>
#include "Core/Log.h"
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
		ModelProgressHandler() = default;
		virtual ~ModelProgressHandler() = default;

		// Inherited via ProgressHandler
		virtual bool Update(float percentage = (-1.0F))
		{
			ZR_CORE_INFO("Model loading progress: {0:.2f}%", percentage * 100.f);
			notify(percentage);
			if (mProgressFunction) {
				mProgressFunction(percentage);
			}

			mProgress = percentage;
			return true;
		}
	};
}
