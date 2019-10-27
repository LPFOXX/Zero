#pragma once

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
		bool setAnimation(const std::string& animationName, bool startAnimation = false);
		bool setAnimation(unsigned animationIndex, bool startAnimation = false);
		bool getAvailableAnimations(std::vector<std::string>& animationNames);
		bool isLoaded();
		bool hasAnimations();

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
