#pragma once

//#include <assimp/scene.h>
//#include <assimp/Logger.hpp>
//#include <assimp/ProgressHandler.hpp>
//#include <assimp/DefaultLogger.hpp>
//#include <assimp/Importer.hpp>
//#include <assimp/material.h>
//#include <assimp/postprocess.h>

//#include <SFML/System.hpp>
//
//#include "Renderer/Texture.h"
//#include "Renderer/Shader.h"
//
//#include "Mesh.h"
//#include "MeshData.h"
//#include "Core/Log.h"
//#include "Core/Core.h"
//#include "Image.h"
//#include "ModelData.h"
//#include "ModelLoader.h"
//
//#include "ProgressHandler.h"

#include "../vendor/glm/include/glm/glm.hpp"
#include "Core/Clock.h"
#include "Core/Core.h"

#include "ModelImpl.h"
#include "ModelData.h"

namespace zr
{
	class Model
	{
	public:
		Model(const std::string& filePath, unsigned componentsToLoad = MeshData::Normals | MeshData::TangentsAndBitangents | MeshData::TextureCoordinates | MeshData::Animations, const ProgressFunction& progressUpdateFunction = nullptr);
		virtual ~Model();

		void setModelTransform(const glm::mat4& modelTransform);
		void update(const Time& elapsedTime);
		void render(const glm::mat4& viewProjectionMatrix);

		float getLoadingProgress() const;

	private:
		void loadModel();

	private:
		std::string mFilePath;
		ProgressFunction mProgressFunction;
		sf::Thread mModelLoadingThread;
		mutable sf::Mutex mProgressHandlerMutex;
		float mLoadingProgress;
		Ref<ModelData> mModelData;
		Scope<ModelImpl> mModelInstance;
		unsigned mComponents;
	};
}
