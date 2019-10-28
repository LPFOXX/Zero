#include <zr_pch.h>

#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>

#include "Core/Log.h"
#include "ImGui/ImGuiConsole.h"

#include "ProgressHandler.h"
#include "ModelLoader.h"
#include "Model.h"
#include "NullModelImpl.h"
#include "Model3D.h"

#include "CommandQueue.h"

namespace zr
{
	Model::Model(const std::string& filePath, unsigned componentsToLoad, const ProgressFunction& progressUpdateFunction) :
		mFilePath(filePath),
		mProgressFunction(progressUpdateFunction),
		mModelLoadingThread(&Model::loadModel, this),
		mProgressHandlerMutex(),
		mLoadingProgress(0.f),
		mModelData(nullptr),
		mModelInstance(new NullModelImpl),
		mComponents(componentsToLoad)
	{
		mModelLoadingThread.launch();
	}

	Model::~Model()
	{
		mModelLoadingThread.wait();
	}

	void Model::setModelTransform(const glm::mat4& modelTransform)
	{
		mModelInstance->setModelTransform(modelTransform);
	}

	void Model::update(const Time& elapsedTime)
	{
		mModelInstance->update(elapsedTime);
	}

	void Model::render(const glm::mat4& viewProjectionMatrix)
	{
		mModelInstance->render(viewProjectionMatrix);
	}

	bool Model::setAnimation(const std::string& animationName, bool startAnimation)
	{
		return mModelInstance->setAnimation(animationName, startAnimation);
	}

	bool Model::setAnimation(unsigned animationIndex, bool startAnimation)
	{
		return mModelInstance->setAnimation(animationIndex, startAnimation);
	}

	bool Model::stopAnimation()
	{
		return mModelInstance->stopAnimation();
	}

	bool Model::getAvailableAnimations(std::vector<std::string>& animationNames)
	{
		return mModelInstance->getAvailableAnimations(animationNames);
	}

	bool Model::isLoaded()
	{
		return mModelInstance->isLoaded();
	}

	bool Model::hasAnimations()
	{
		return mModelInstance->hasAnimations();
	}

	float Model::getLoadingProgress() const
	{
		sf::Lock l(mProgressHandlerMutex);
		return mLoadingProgress;
	}

	void Model::loadModel()
	{
		Assimp::Importer importer;
		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

		const Ref<ModelProgressHandler>& progressHandler = ModelLoader::Get()->getProgressHandler();
		if (progressHandler != nullptr) {
			ModelProgressHandler* newProgressHandler = new ModelProgressHandler(*progressHandler);
			//newProgressHandler->setUpdateFunction(mProgressFunction);
			newProgressHandler->setUpdateFunction([&](float& progress) {
				sf::Lock l(mProgressHandlerMutex);
				mLoadingProgress = progress;
			});
			importer.SetProgressHandler(dynamic_cast<Assimp::ProgressHandler*>(newProgressHandler));
			#ifdef ZR_DEBUG_BUILD
			importer.SetExtraVerbose(true); // debug feature
			#endif
		}
		unsigned flags =
			(mComponents & MeshData::TangentsAndBitangents ? aiProcess_CalcTangentSpace : 0U) |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_RemoveComponent |
			aiProcess_SplitLargeMeshes |
			aiProcess_ValidateDataStructure |
			aiProcess_ImproveCacheLocality |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_SortByPType |
			aiProcess_FindInvalidData |
			aiProcess_OptimizeMeshes |
			aiProcess_OptimizeGraph |
			aiProcess_FlipUVs;

		mModelData.reset(new ModelData());
		if (!mModelData->loadFromFile(importer, mFilePath, flags, mComponents)) {
			ZR_CORE_ERROR("[Model] Can't load model from '{0}'", mFilePath);
			ZR_IMGUI_LOG(ConsoleItem::Error, "[Model] Can't load model from '%s'", mFilePath.c_str());
		}
		else {
			CommandQueue::Enqueue([&]() {
				mModelInstance.reset(new Model3D(mFilePath, mModelData));
				mModelData.reset();
			});
			ZR_IMGUI_LOG(ConsoleItem::Info, "[Model] Finished loading model data from '%s'", mFilePath.c_str());
		}
	}
}
