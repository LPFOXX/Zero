#pragma once

#include "ProgressHandler.h"
#include "ModelLogger.h"

namespace zr
{
	class ModelLoader
	{
	public:
		ModelLoader() = default;
		virtual ~ModelLoader()
		{
			Assimp::DefaultLogger::kill();
		}

		//void loadFromFile(const std::string& filePath, unsigned componentsToLoad = MeshData::Normals | MeshData::TangentsAndBitangents | MeshData::TextureCoordinates, const ProgressFunction& progressUpdateFunction = nullptr)
		//{
		//	sf::Thread modelLoadingThread([&]() {
		//		Ref<ModelData> modelData = std::make_shared<ModelData>();
		//		Assimp::Importer importer;
		//		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);

		//		//ModelProgressHandler* progressHandler = new ModelProgressHandler;
		//		/*progressHandler->setUpdateFunction(progressUpdateFunction);*/
		//		//progressHandler->subscribe(static_cast<Ref<Observer<float>>>(progressListener));
		//		if (mProgressHandler) {
		//			ModelProgressHandler* newProgressHandler = new ModelProgressHandler(*mProgressHandler);
		//			newProgressHandler->setUpdateFunction(progressUpdateFunction);
		//			importer.SetProgressHandler(dynamic_cast<Assimp::ProgressHandler*>(newProgressHandler));
		//			#ifdef ZR_DEBUG_BUILD
		//			importer.SetExtraVerbose(true); // debug feature
		//			#endif
		//		}
		//		unsigned flags = (componentsToLoad & MeshData::TangentsAndBitangents ? aiProcess_CalcTangentSpace : 0U) | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_SortByPType | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipUVs;
		//		if (!modelData->loadFromFile(importer, filePath, componentsToLoad)) {
		//			ZR_CORE_ERROR("[Model] Can't load model from '{0}'", filePath);
		//		}

		//		ZR_CORE_INFO("Finished loading model data from '{0}'", filePath);
		//		modelData->setFinishedLoading();

		//		//mModelDataMap[filePath] = modelData;
		//	});
		//	modelLoadingThread.launch();
		//	//aiProcess_RemoveComponent AI_CONFIG_PP_RVC_FLAGS
		//	// aiProcess_SplitLargeMeshes AI_CONFIG_PP_SLM_VERTEX_LIMIT (AI_SLM_DEFAULT_MAX_VERTICES) or AI_CONFIG_PP_SLM_TRIANGLE_LIMIT (AI_SLM_DEFAULT_MAX_TRIANGLES)
		//	//return std::make_shared<Model3D>(importer, filePath, flags, componentsToLoad);
		//	return;
		//}

		void setProgressHandler(const Ref<ModelProgressHandler>& progressHandler)
		{
			mProgressHandler = progressHandler;
		}

		const Ref<ModelProgressHandler>& getProgressHandler() const
		{
			return mProgressHandler;
		}

		void setLogger(const Ref<ModelLogger>& logger)
		{
			mLogger = logger;
			ModelLogger* newLogger = new ModelLogger(*mLogger);
			Assimp::DefaultLogger::set(dynamic_cast<Assimp::Logger*>(&*newLogger));
			Assimp::DefaultLogger::get()->info("Custom Logger Created");
		}

	public:
		static const Ref<ModelLoader>& Get()
		{
			{
				sf::Lock lock(ModelLoader::sInstanceMutex);
				if (ModelLoader::sInstance == nullptr) {
					ModelLoader::sInstance = std::make_shared<ModelLoader>();
				}
			}
			return ModelLoader::sInstance;
		}

	private:


	private:
		Ref<ModelProgressHandler> mProgressHandler;
		Ref<ModelLogger> mLogger;
		static sf::Mutex sInstanceMutex;
		static Ref<ModelLoader> sInstance;
	};
}