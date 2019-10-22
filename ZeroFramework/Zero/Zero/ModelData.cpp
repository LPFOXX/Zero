#include "ModelData.h"
#include <zr_pch.h>

#include "Core/Log.h"
#include "Core/Timer.h"
#include "ImGui/ImGuiConsole.h"

#include <assimp/Importer.hpp>

#include "ModelData.h"

namespace zr
{
	bool ModelData::loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned& componentsToLoad)
	{
		Timer t("ModelData::loadFromFile");
		const aiScene* scene = importer.ReadFile(filePath.c_str(), flags);
		if (!scene) {
			ZR_CORE_ERROR("[ASSIMP] {0}", importer.GetErrorString());
			ZR_IMGUI_LOG(ConsoleItem::Error, "[ASSIMP] %s", importer.GetErrorString());
			return false;
		}

		mDirectory = filePath.substr(0, filePath.find_last_of("/\\") + 1);
		updateFlag(componentsToLoad, MeshData::Textures, scene->HasTextures());
		updateFlag(componentsToLoad, MeshData::Materials, scene->HasMaterials());
		updateFlag(componentsToLoad, MeshData::Animations, scene->HasAnimations());

		mComponents = componentsToLoad;

		walkThroughChildren(scene->mRootNode, scene);
		return true;
	}

	void ModelData::walkThroughChildren(const aiNode* rootNode, const aiScene* scene)
	{
		if (rootNode->mNumChildren > 0) {
			for (unsigned i = 0; i < rootNode->mNumChildren; ++i) {
				walkThroughChildren(rootNode->mChildren[i], scene);
			}
		}
		else {
			for (unsigned i = 0; i < rootNode->mNumMeshes; ++i) {
				mMeshesData.emplace_back(processMesh(scene->mMeshes[rootNode->mMeshes[i]], scene));
			}
		}
	}

	MeshData* ModelData::processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<MeshData::Vertex> vertices;
		std::vector<unsigned> indices;
		std::unordered_map<Texture2D::Type, std::vector<std::string>> textures;
		std::map<std::string, MeshData::Bone> meshBones;

		bool meshHasNormals = mesh->HasNormals() && (mComponents & MeshData::Normals);
		bool meshHasTangentsAndBitangents = mesh->HasTangentsAndBitangents() && (mComponents & MeshData::TangentsAndBitangents);
		bool meshHasTextureCoordinates = mesh->HasTextureCoords(0) && (mComponents & MeshData::TextureCoordinates);
		bool meshHasBones = mesh->HasBones() && (mComponents & MeshData::Animations);
		bool searchForMaterials = mComponents & MeshData::Materials;
		bool searchForTextures = mComponents & MeshData::Textures;
		bool meshHasFaces = mesh->HasFaces();

		MeshData::MeshProperties props;
		props.Name = mesh->mName.C_Str();
		updateFlag(props.Components, MeshData::Normals, meshHasNormals);
		updateFlag(props.Components, MeshData::TangentsAndBitangents, meshHasTangentsAndBitangents);
		updateFlag(props.Components, MeshData::TextureCoordinates, meshHasTextureCoordinates);
		updateFlag(props.Components, MeshData::Textures, searchForTextures);
		updateFlag(props.Components, MeshData::Materials, searchForMaterials);
		updateFlag(props.Components, MeshData::Animations, meshHasBones);

		// Loads the vertex positions values into the vector.
		// While doing it a vertex object is created.
		// So, in order to fill the remaining data (normals, texture coordinates, tangents
		// and bitangent) we just need to access the element and copy the data).
		for (unsigned vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			MeshData::Vertex newVertex;

			// Positions
			newVertex.Position.x = mesh->mVertices[vertexIndex].x;
			newVertex.Position.y = mesh->mVertices[vertexIndex].y;
			newVertex.Position.z = mesh->mVertices[vertexIndex].z;

			// Gets normal vectors
			if (meshHasNormals) {
				newVertex.Normal.x = mesh->mNormals[vertexIndex].x;
				newVertex.Normal.y = mesh->mNormals[vertexIndex].y;
				newVertex.Normal.z = mesh->mNormals[vertexIndex].z;
			}

			// Gets tangents and bitangents vectors
			if (meshHasTangentsAndBitangents) {
				// Tangents
				newVertex.Tangent.x = mesh->mTangents[vertexIndex].x;
				newVertex.Tangent.y = mesh->mTangents[vertexIndex].y;
				newVertex.Tangent.z = mesh->mTangents[vertexIndex].z;
				// Bitangents
				newVertex.Bitangent.x = mesh->mBitangents[vertexIndex].x;
				newVertex.Bitangent.y = mesh->mBitangents[vertexIndex].y;
				newVertex.Bitangent.z = mesh->mBitangents[vertexIndex].z;
			}

			// Gets texture coordinates
			if (meshHasTextureCoordinates) {
				newVertex.TextureCoordinates.x = mesh->mTextureCoords[0][vertexIndex].x;
				newVertex.TextureCoordinates.y = mesh->mTextureCoords[0][vertexIndex].y;
			}

			vertices.push_back(newVertex);
		}

		// Process the faces (the primitives that forms a face) a set of several points.
		// Those are the indices.
		if (meshHasFaces) {
			for (unsigned facesIndex = 0; facesIndex < mesh->mNumFaces; facesIndex++) {
				const aiFace& face = mesh->mFaces[facesIndex];

				for (unsigned indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++) {
					indices.push_back(face.mIndices[indicesIndex]);
				}
			}
		}

		// Gets bones
		if (meshHasBones) {
			// mesh->mBones[vertexIndex];
			for (unsigned bonesIndex = 0; bonesIndex < mesh->mNumBones; bonesIndex++) {
				const aiBone* bone = mesh->mBones[bonesIndex];
				ZR_IMGUI_LOG(ConsoleItem::Info, "Bone name: %s", bone->mName.C_Str());
				MeshData::Bone newBone(bone->mName.data);
				newBone.OffsetMatrix = glm::mat4(
					{ bone->mOffsetMatrix[0][0], bone->mOffsetMatrix[1][0], bone->mOffsetMatrix[2][0], bone->mOffsetMatrix[3][0] },
					{ bone->mOffsetMatrix[0][1], bone->mOffsetMatrix[1][1], bone->mOffsetMatrix[2][1], bone->mOffsetMatrix[3][1] },
					{ bone->mOffsetMatrix[0][2], bone->mOffsetMatrix[1][2], bone->mOffsetMatrix[2][2], bone->mOffsetMatrix[3][2] },
					{ bone->mOffsetMatrix[0][3], bone->mOffsetMatrix[1][3], bone->mOffsetMatrix[2][3], bone->mOffsetMatrix[3][3] });
				for (unsigned boneWeightsIndex = 0; boneWeightsIndex < bone->mNumWeights; boneWeightsIndex++) {
					newBone.Weights.emplace_back(bone->mWeights[boneWeightsIndex].mVertexId, bone->mWeights[boneWeightsIndex].mWeight);
				}
				meshBones[newBone.Name] = std::move(newBone);
			}
		}

		if (searchForMaterials) {
			// process materials
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// diffuse: texture_diffuseN
			// specular: texture_specularN
			// normal: texture_normalN

			aiColor4D ambient, diffuse, specular, emissive;
			float shininess = 0.f;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);
			aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

			props.mMaterial.Ambient = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
			props.mMaterial.Diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			props.mMaterial.Specular = glm::vec4(specular.r, specular.g, specular.b, specular.a);
			props.mMaterial.Emissive = glm::vec4(emissive.r, emissive.g, emissive.b, emissive.a);
			props.mMaterial.Shininess = shininess;

			if (searchForTextures) {
				// 1. ambient maps
				std::vector<std::string>& ambientMap = processMaterial(material, Texture2D::Type::Ambient);
				if (!ambientMap.empty()) {
					textures[Texture2D::Type::Ambient] = ambientMap;
				}

				// 2. diffuse maps 
				std::vector<std::string>& diffuseMap = processMaterial(material, Texture2D::Type::Diffuse);
				if (!diffuseMap.empty()) {
					textures[Texture2D::Type::Diffuse] = diffuseMap;
				}

				// 3. specular maps
				std::vector<std::string>& specularMap = processMaterial(material, Texture2D::Type::Specular);
				if (!specularMap.empty()) {
					textures[Texture2D::Type::Specular] = specularMap;
				}

				// 4. normal maps
				std::vector<std::string>& normalMap = processMaterial(material, Texture2D::Type::Normal);
				if (!normalMap.empty()) {
					textures[Texture2D::Type::Normal] = normalMap;
				}

				// 5. height maps
				std::vector<std::string>& heightMap = processMaterial(material, Texture2D::Type::Height);
				if (!heightMap.empty()) {
					textures[Texture2D::Type::Height] = heightMap;
				}
			}
		}

		// return a meshdata object created from the extracted mesh data
		return new MeshData(props, vertices, indices, textures);
	}

	std::vector<std::string> ModelData::processMaterial(const aiMaterial* material, Texture2D::Type textureType)
	{
		std::vector<std::string> meshTexturesPath;

		aiTextureType type;
		switch (textureType) {
			case zr::Texture2D::Type::Ambient:
			{
				type = aiTextureType_AMBIENT;
				break;
			}
			case zr::Texture2D::Type::None:
			{
				type = aiTextureType_NONE;
				break;
			}
			case zr::Texture2D::Type::Diffuse:
			{
				type = aiTextureType_DIFFUSE;
				break;
			}
			case zr::Texture2D::Type::Specular:
			{
				type = aiTextureType_SPECULAR;
				break;
			}
			case zr::Texture2D::Type::Normal:
			{
				type = aiTextureType_NORMALS;
				break;
			}
			case zr::Texture2D::Type::Height:
			{
				type = aiTextureType_HEIGHT;
				break;
			}
			default:
			{
				type = aiTextureType_UNKNOWN;
				break;
			}
		}
		/*
		aiTextureType_EMISSIVE
		aiTextureType_SHININESS
		aiTextureType_OPACITY
		aiTextureType_DISPLACEMENT
		aiTextureType_LIGHTMAP
		aiTextureType_REFLECTION*/


		unsigned materialCount = material->GetTextureCount(type);

		for (unsigned i = 0; i < materialCount; i++) {
			aiString texturePath;
			material->GetTexture(type, i, &texturePath);

			// check if texture was loaded before
			auto found = std::find_if(mLoadedTextures.begin(), mLoadedTextures.end(), [&](const Image& image) {
				return (mDirectory + std::string(texturePath.C_Str())) == image.getPath();
			});

			if (found == mLoadedTextures.end()) {
				// Not loaded yet: load it
				Image image;
				image.loadFromFile(mDirectory + texturePath.C_Str());
				mLoadedTextures.emplace_back(image);
			}
			meshTexturesPath.emplace_back(mDirectory + texturePath.C_Str());
		}

		return meshTexturesPath;
	}

	void ModelData::updateFlag(unsigned& flags, unsigned bit, bool hasComponent) const
	{
		if (hasComponent) {
			// Add component flag
			flags |= bit;
		}
		else {
			// Remove component flag
			flags &= ~(unsigned)bit;
		}
	}
}
