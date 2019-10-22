#pragma once

#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include <SFML/System.hpp>

#include "Core/Core.h"
#include "Core/Log.h"

#include "Image.h"
#include "Renderer/Texture.h"
#include "MeshData.h"

namespace zr
{
	class ModelData
	{
	public:
		ModelData() = default;
		virtual ~ModelData() = default;

		bool loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned& componentsToLoad);

		void walkThroughChildren(const aiNode* rootNode, const aiScene* scene);

		MeshData* processMesh(const aiMesh* mesh, const aiScene* scene);

		std::vector<std::string> processMaterial(const aiMaterial* material, Texture2D::Type textureType);

		inline const std::vector<Scope<MeshData>>& getMeshesData()
		{
			return mMeshesData;
		}

		inline unsigned getComponents() const
		{
			return mComponents;
		}

		inline const std::vector<Image>& getTextures() const
		{
			return mLoadedTextures;
		}

	private:
		void updateFlag(unsigned& flags, unsigned bit, bool hasComponent) const;

	private:
		std::vector<Scope<MeshData>> mMeshesData;
		std::vector<Image> mLoadedTextures;
		std::string mDirectory;
		unsigned mComponents;
	};
}
