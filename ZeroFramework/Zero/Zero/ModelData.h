//#pragma once
//
//#include <assimp/scene.h>
//#include <assimp/material.h>
//#include <assimp/postprocess.h>
//
//#include <SFML/System.hpp>
//
//#include "Core/Core.h"
//#include "Core/Log.h"
//
//#include "Renderer/Texture.h"
//
//#include "MeshData.h"
//
//namespace zr
//{
//	class ModelData
//	{
//	public:
//		ModelData() = default;
//		virtual ~ModelData() = default;
//
//		bool loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned componentsToLoad = MeshData::Normals | MeshData::TangentsAndBitangents | MeshData::TextureCoordinates);
//
//		void walkThroughChildren(const aiNode* rootNode, const aiScene* scene);
//
//		MeshData* processMesh(const aiMesh* mesh, const aiScene* scene);
//
//		std::vector<Image> processMaterial(const aiMaterial* material, Texture2D::Type textureType);
//
//		inline const std::vector<Scope<MeshData>>& getMeshesData()
//		{
//			return mMeshesData;
//		}
//
//		inline bool isFinishedLoading()
//		{
//			sf::Lock lock(mObjectMutex);
//			return mIsFinishedLoading;
//		}
//
//	private:
//		std::vector<Scope<MeshData>> mMeshesData;
//		std::vector<std::string> mTexturesLoadedPath;
//		std::string mDirectory;
//		bool mIsFinishedLoading = false;
//		unsigned mComponents = 0;
//
//		sf::Mutex mObjectMutex;
//	};
//}
