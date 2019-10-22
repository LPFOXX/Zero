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
	//	sf::Mutex ModelLoader::sInstanceMutex;
	//	Ref<ModelLoader> ModelLoader::sInstance = nullptr;
	//
	//	/*void Model3D::walkThroughChildren(const aiNode* rootNode, const aiScene* scene)
	//	{
	//		if (rootNode->mNumChildren != 0) {
	//			for (unsigned i = 0; i < rootNode->mNumChildren; i++) {
	//				for (unsigned j = 0; j < rootNode->mChildren[i]->mNumMeshes; j++) {
	//					mMeshes.emplace_back(processMesh(scene->mMeshes[rootNode->mChildren[i]->mMeshes[j]], scene));
	//				}
	//
	//				walkThroughChildren(rootNode->mChildren[i], scene);
	//			}
	//		}
	//		else {
	//			for (unsigned i = 0; i < rootNode->mNumMeshes; i++) {
	//				mMeshes.emplace_back(processMesh(scene->mMeshes[rootNode->mMeshes[i]], scene));
	//			}
	//		}
	//	}*/
	//
	//	//Mesh* Model3D::processMesh(const aiMesh* mesh, const aiScene* scene)
	//	//{
	//	//	std::vector<MeshData::Vertex> vertices;
	//	//	std::vector<unsigned> indices;
	//	//	std::vector<Ref<Texture2D>> textures;
	//
	//	//	bool meshHasNormals = mesh->HasNormals() && (mComponents & MeshData::Normals);
	//	//	bool meshHasTangentsAndBitangents = mesh->HasTangentsAndBitangents() && (mComponents & MeshData::TangentsAndBitangents);
	//	//	bool meshHasTextureCoordinates = mesh->HasTextureCoords(0) && (mComponents & MeshData::TextureCoordinates);
	//	//	bool meshHasFaces = mesh->HasFaces();
	//
	//	//	// Loads the vertex positions values into the vector.
	//	//	// While doing it a vertex object is created.
	//	//	// So, in order to fill the remaining data (normals, texture coordinates, tangents
	//	//	// and bitangent) we just need to access the element and copy the data).
	//	//	for (unsigned vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
	//	//		MeshData::Vertex newVertex;
	//
	//	//		// Positions
	//	//		newVertex.Position.x = mesh->mVertices[vertexIndex].x;
	//	//		newVertex.Position.y = mesh->mVertices[vertexIndex].y;
	//	//		newVertex.Position.z = mesh->mVertices[vertexIndex].z;
	//
	//	//		// Gets normal vectors
	//	//		if (meshHasNormals) {
	//	//			newVertex.Normal.x = mesh->mNormals[vertexIndex].x;
	//	//			newVertex.Normal.y = mesh->mNormals[vertexIndex].y;
	//	//			newVertex.Normal.z = mesh->mNormals[vertexIndex].z;
	//	//		}
	//
	//	//		// Gets tangents and bitangents vectors
	//	//		if (meshHasTangentsAndBitangents) {
	//	//			// Tangents
	//	//			newVertex.Tangent.x = mesh->mTangents[vertexIndex].x;
	//	//			newVertex.Tangent.y = mesh->mTangents[vertexIndex].y;
	//	//			newVertex.Tangent.z = mesh->mTangents[vertexIndex].z;
	//	//			// Bitangents
	//	//			newVertex.Bitangent.x = mesh->mBitangents[vertexIndex].x;
	//	//			newVertex.Bitangent.y = mesh->mBitangents[vertexIndex].y;
	//	//			newVertex.Bitangent.z = mesh->mBitangents[vertexIndex].z;
	//	//		}
	//
	//	//		// Gets texture coordinates
	//	//		if (meshHasTextureCoordinates) {
	//	//			newVertex.TextureCoordinates.x = mesh->mTextureCoords[0][vertexIndex].x;
	//	//			newVertex.TextureCoordinates.y = mesh->mTextureCoords[0][vertexIndex].y;
	//	//		}
	//
	//	//		vertices.push_back(newVertex);
	//	//	}
	//
	//	//	// Process the faces (the primitives that forms a face) a set of several points.
	//	//	// Those are the indices.
	//	//	if (meshHasFaces) {
	//	//		for (unsigned facesIndex = 0; facesIndex < mesh->mNumFaces; facesIndex++) {
	//	//			const aiFace& face = mesh->mFaces[facesIndex];
	//
	//	//			for (unsigned indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++) {
	//	//				indices.push_back(face.mIndices[indicesIndex]);
	//	//			}
	//	//		}
	//	//	}
	//
	//	//	// process materials
	//	//	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	//	//	// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
	//	//	// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
	//	//	// Same applies to other texture as the following list summarizes:
	//	//	// diffuse: texture_diffuseN
	//	//	// specular: texture_specularN
	//	//	// normal: texture_normalN
	//
	//	//	aiColor4D ambient, diffuse, specular, emissive;
	//	//	float shininess = 0.f;
	//	//	aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
	//	//	aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
	//	//	aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
	//	//	aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);
	//	//	aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
	//
	//	//	MeshData::MeshProperties props;
	//	//	props.mMaterial.Ambient.r = ambient.r;
	//	//	props.mMaterial.Ambient.g = ambient.g;
	//	//	props.mMaterial.Ambient.b = ambient.b;
	//	//	props.mMaterial.Ambient.a = ambient.a;
	//
	//	//	props.mMaterial.Diffuse.r = diffuse.r;
	//	//	props.mMaterial.Diffuse.g = diffuse.g;
	//	//	props.mMaterial.Diffuse.b = diffuse.b;
	//	//	props.mMaterial.Diffuse.a = diffuse.a;
	//
	//	//	props.mMaterial.Specular.r = specular.r;
	//	//	props.mMaterial.Specular.g = specular.g;
	//	//	props.mMaterial.Specular.b = specular.b;
	//	//	props.mMaterial.Specular.a = specular.a;
	//
	//	//	props.mMaterial.Emissive.r = emissive.r;
	//	//	props.mMaterial.Emissive.g = emissive.g;
	//	//	props.mMaterial.Emissive.b = emissive.b;
	//	//	props.mMaterial.Emissive.a = emissive.a;
	//
	//	//	props.mMaterial.Shininess = shininess;
	//
	//	//	// 1. ambient maps
	//	//	std::vector<Ref<Texture2D>> ambientMaps = processMaterial(material, Texture2D::Type::Ambient);
	//	//	textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
	//	//	// 2. diffuse maps
	//	//	std::vector<Ref<Texture2D>> diffuseMaps = processMaterial(material, Texture2D::Type::Diffuse);
	//	//	textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	//	//	// 2. specular maps
	//	//	std::vector<Ref<Texture2D>> specularMaps = processMaterial(material, Texture2D::Type::Specular);
	//	//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	//	//	// 3. normal maps
	//	//	std::vector<Ref<Texture2D>> normalMaps = processMaterial(material, Texture2D::Type::Normal);
	//	//	textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	//	//	// 4. height maps
	//	//	std::vector<Ref<Texture2D>> heightMaps = processMaterial(material, Texture2D::Type::Height);
	//	//	textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	//
	//	//	mMaxAmbientTextures = std::max(mMaxAmbientTextures, static_cast<unsigned>(ambientMaps.size()));
	//	//	mMaxDiffuseTextures = std::max(mMaxDiffuseTextures, static_cast<unsigned>(diffuseMaps.size()));
	//	//	mMaxSpecularTextures = std::max(mMaxSpecularTextures, static_cast<unsigned>(specularMaps.size()));
	//	//	mMaxNormalTextures = std::max(mMaxNormalTextures, static_cast<unsigned>(normalMaps.size()));
	//	//	mMaxHeightTextures = std::max(mMaxHeightTextures, static_cast<unsigned>(heightMaps.size()));
	//
	//	//	props.Components |= meshHasNormals ? MeshData::MeshProperties::BitField::Normals : 0U;
	//	//	props.Components |= meshHasTangentsAndBitangents ? MeshData::MeshProperties::BitField::TangentsAndBitangents : 0U;
	//	//	props.Components |= meshHasTextureCoordinates ? MeshData::MeshProperties::BitField::TextureCoordinates : 0U;
	//	//	props.Components |= textures.size() != 0U ? MeshData::MeshProperties::BitField::Textures : 0U;
	//
	//	//	props.Name = mesh->mName.C_Str();
	//
	//	//	// return a mesh object created from the extracted mesh data
	//	//	return new Mesh(props, vertices, indices, textures);
	//	//}
	//
	//	//std::vector<Ref<Texture2D>> Model3D::processMaterial(const aiMaterial* material, Texture2D::Type textureType)
	//	//{
	//	//	std::vector<Ref<Texture2D>> textures;
	//
	//	//	aiTextureType type;
	//	//	switch (textureType) {
	//	//		case zr::Texture2D::Type::Ambient:
	//	//		{
	//	//			type = aiTextureType_AMBIENT;
	//	//			break;
	//	//		}
	//	//		case zr::Texture2D::Type::None:
	//	//		{
	//	//			type = aiTextureType_NONE;
	//	//			break;
	//	//		}
	//	//		case zr::Texture2D::Type::Diffuse:
	//	//		{
	//	//			type = aiTextureType_DIFFUSE;
	//	//			break;
	//	//		}
	//	//		case zr::Texture2D::Type::Specular:
	//	//		{
	//	//			type = aiTextureType_SPECULAR;
	//	//			break;
	//	//		}
	//	//		case zr::Texture2D::Type::Normal:
	//	//		{
	//	//			type = aiTextureType_NORMALS;
	//	//			break;
	//	//		}
	//	//		case zr::Texture2D::Type::Height:
	//	//		{
	//	//			type = aiTextureType_HEIGHT;
	//	//			break;
	//	//		}
	//	//		default:
	//	//		{
	//	//			type = aiTextureType_UNKNOWN;
	//	//			break;
	//	//		}
	//	//	}
	//	//	/*
	//	//	aiTextureType_EMISSIVE
	//	//	aiTextureType_SHININESS
	//	//	aiTextureType_OPACITY
	//	//	aiTextureType_DISPLACEMENT
	//	//	aiTextureType_LIGHTMAP
	//	//	aiTextureType_REFLECTION*/
	//
	//
	//	//	unsigned materialCount = material->GetTextureCount(type);
	//
	//	//	for (unsigned i = 0; i < materialCount; i++) {
	//	//		aiString texturePath;
	//	//		material->GetTexture(type, i, &texturePath);
	//
	//	//		// check if texture was loaded before
	//	//		auto found = std::find_if(mTexturesLoadedPath.begin(), mTexturesLoadedPath.end(), [&texturePath](const std::string& loadedTexturePath) {
	//	//			return std::strcmp(loadedTexturePath.c_str(), texturePath.C_Str()) == 0;
	//	//		});
	//
	//	//		if (found == mTexturesLoadedPath.end()) {
	//	//			// Not found: load it
	//	//			mTexturesLoadedPath.push_back(texturePath.C_Str());
	//	//			textures.emplace_back(Texture2D::Create(mDirectory + texturePath.C_Str(), textureType));
	//	//		}
	//	//	}
	//
	//	//	for (auto& texture : textures) {
	//	//		texture->generateMipMaps();
	//	//	}
	//	//	return textures;
	//	//}
	//
	//	
}
