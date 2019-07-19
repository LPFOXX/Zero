#pragma once

#include <assimp/scene.h>
#include "Renderer/Texture.h"
#include "Renderer/Shader.h"

#include "Mesh.h"

namespace zr
{
	class Model
	{
	public:
		enum LoadComponents
		{
			Normals =					1 << 0,
			TangentsAndBitangents =		1 << 1,
			TextureCoordinates =		1 << 2
		};

	public:
		Model(const std::string& filePath, unsigned componentsToLoad = Normals | TangentsAndBitangents | TextureCoordinates);
		virtual ~Model();

		void setTransformationMatrix(const glm::mat4& transformations);

		void render(const glm::mat4& viewProjectionMatrix);

	private:
		void walkThroughChildren(const aiNode* rootNode, const aiScene* scene);
		Mesh* processMesh(const aiMesh* mesh, const aiScene* scene);
		std::vector<std::shared_ptr<Texture>> processMaterial(const aiMaterial* material, Texture::TextureType textureType);
		void generateShader();

	private:
		std::vector<std::unique_ptr<Mesh>> mMeshes;
		std::shared_ptr<Shader> mShader;
		std::vector<std::string> mTexturesLoadedPath;
		std::string mDirectory;

		glm::mat4 mTransformationMatrix;

		unsigned mMaxDiffuseTextures = 0;
		unsigned mMaxSpecularTextures = 0;
		unsigned mMaxNormalTextures = 0;
		unsigned mMaxHeightTextures = 0;

		unsigned mComponents = 0;
	};
}
