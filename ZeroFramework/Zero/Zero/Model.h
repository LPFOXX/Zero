//#pragma once
//
//#include <assimp/scene.h>
//#include <assimp/Logger.hpp>
//#include <assimp/ProgressHandler.hpp>
//#include <assimp/DefaultLogger.hpp>
//#include <assimp/Importer.hpp>
//#include <assimp/material.h>
//#include <assimp/postprocess.h>
//
//#include <SFML/System.hpp>
//
//#include "Renderer/Texture.h"
//#include "Renderer/Shader.h"
//
//#include "Mesh.h"
//#include "Core/Log.h"
//#include "Core/Clock.h"
//#include "Core/Core.h"
//#include "Image.h"
//#include "ModelData.h"
//#include "ModelLoader.h"
//
//#include "ProgressHandler.h"
//
//namespace zr
//{
//	class Model
//	{
//	public:
//		Model() = default;
//		virtual ~Model() = default;
//
//		virtual void update(const Time& elapsedTime) = 0;
//		virtual void render(const glm::mat4& viewProjectionMatrix) = 0;
//	};
//
//	class NullModel : public Model
//	{
//	public:
//		NullModel() = default;
//		virtual ~NullModel() = default;
//
//		virtual void render(const glm::mat4& viewProjectionMatrix) override
//		{
//
//		}
//	};
//
//	struct ModelLoadingThreadData
//	{
//		ModelLoadingThreadData(const std::string& filePath, unsigned componentsToLoad, const ProgressFunction& progressUpdateFunction) :
//			FilePath(filePath),
//			ComponentsToLoad(componentsToLoad),
//			ProgressUpdateFunction(progressUpdateFunction)
//		{
//		}
//
//		std::string FilePath;
//		unsigned ComponentsToLoad;
//		ProgressFunction ProgressUpdateFunction;
//	};
//
//	class Model3D : public Model
//	{
//	public:
//		Model3D(const std::string& filePath, unsigned componentsToLoad = MeshData::Normals | MeshData::TangentsAndBitangents | MeshData::TextureCoordinates, const ProgressFunction& progressUpdateFunction = nullptr);
//		virtual ~Model3D();
//
//		void setTransformationMatrix(const glm::mat4& transformations);
//		void setCameraPosition(const glm::vec3& cameraPosition)
//		{
//			mCameraPosition = cameraPosition;
//		}
//
//		void render(const glm::mat4& viewProjectionMatrix);
//
//	private:
//		void loadFromFile();
//		void generateBuffers();
//
//		//void walkThroughChildren(const aiNode* rootNode, const aiScene* scene);
//		//Mesh* processMesh(const aiMesh* mesh, const aiScene* scene);
//		//std::vector<Ref<Texture2D>> processMaterial(const aiMaterial* material, Texture2D::Type textureType);
//		void generateShader();
//
//	private:
//		sf::Thread mModelLoadingThread;
//		Ref<ModelData> mModelData;
//		ProgressFunction mProgressFunction;
//		std::string mFilePath;
//		bool mIsLoaded;
//
//		std::vector<Ref<Mesh>> mMeshes;
//		std::shared_ptr<Shader> mShader;
//		std::vector<std::string> mTexturesLoadedPath;
//		std::string mDirectory;
//
//		glm::mat4 mTransformationMatrix;
//
//		unsigned mMaxAmbientTextures = 0;
//		unsigned mMaxDiffuseTextures = 0;
//		unsigned mMaxSpecularTextures = 0;
//		unsigned mMaxNormalTextures = 0;
//		unsigned mMaxHeightTextures = 0;
//
//		unsigned mComponents = 0;
//
//		zr::DirectionalLight mDirectionalLight;
//		glm::vec3 mCameraPosition = glm::vec3(0.f, 0.f, 0.f);
//
//		// Inherited via Model
//		virtual void update(const Time& elapsedTime) override;
//	};
//}
