#pragma once

#include <assimp/DefaultLogger.hpp>
#include "../vendor/sfml/include/SFML/System.hpp"

#include "ProgressHandler.h"
#include "ModelLogger.h"

namespace zr
{
	class ModelLoader
	{
	public:
		ModelLoader() = default;
		virtual ~ModelLoader();

		void setProgressHandler(const Ref<ModelProgressHandler>& progressHandler);
		const Ref<ModelProgressHandler>& getProgressHandler() const;
		void setLogger(const Ref<ModelLogger>& logger);

	public:
		static const Ref<ModelLoader>& Get();

	private:
		static sf::Mutex sInstanceMutex;
		static Ref<ModelLoader> sInstance;

	private:
		Ref<ModelProgressHandler> mProgressHandler;
		Ref<ModelLogger> mLogger;
	};
}
