#include <zr_pch.h>

#include "ModelLoader.h"

namespace zr
{
	sf::Mutex ModelLoader::sInstanceMutex;
	Ref<ModelLoader> ModelLoader::sInstance = nullptr;

	ModelLoader::~ModelLoader()
	{
		Assimp::DefaultLogger::kill();
	}

	void ModelLoader::setProgressHandler(const Ref<ModelProgressHandler>& progressHandler)
	{
		mProgressHandler = progressHandler;
	}

	const Ref<ModelProgressHandler>& ModelLoader::getProgressHandler() const
	{
		return mProgressHandler;
	}

	void ModelLoader::setLogger(const Ref<ModelLogger>& logger)
	{
		mLogger = logger;
		ModelLogger* newLogger = new ModelLogger(*mLogger);
		Assimp::DefaultLogger::set(dynamic_cast<Assimp::Logger*>(&*newLogger));
		Assimp::DefaultLogger::get()->info("Custom Logger Created");
	}

	const Ref<ModelLoader>& ModelLoader::Get()
	{
		sf::Lock lock(ModelLoader::sInstanceMutex);
		if (ModelLoader::sInstance == nullptr) {
			ModelLoader::sInstance = std::make_shared<ModelLoader>();
		}
		return ModelLoader::sInstance;
	}
};
