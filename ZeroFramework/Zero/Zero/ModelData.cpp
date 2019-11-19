#include <zr_pch.h>

#include <numeric>

#include "Core/Log.h"
#include "Core/Timer.h"
#include "ImGui/ImGuiConsole.h"

#include <assimp/Importer.hpp>

#include "ModelData.h"

namespace zr
{
	bool ModelData::loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned& componentsToLoad)
	{
		Timer<std::function<void(const ProfileResult&)>> t("ModelData::loadFromFile", [] (const ProfileResult& result){});
		importer.SetPropertyBool(AI_CONFIG_GLOB_MEASURE_TIME, true);
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

		Ref<SceneObject> sceneRootNode = std::make_shared<SceneObject>();
		mScene = std::make_shared<Scene>(sceneRootNode);
		walkThroughChildren(scene, scene->mRootNode, sceneRootNode, glm::mat4(1.f));
		if (scene->HasAnimations()) {
			for (unsigned animationsIndex = 0; animationsIndex < scene->mNumAnimations; ++animationsIndex) {
				const aiAnimation* animation = scene->mAnimations[animationsIndex];
				Animation an(animation->mName.data);
				an.setDuration(animation->mDuration, animation->mTicksPerSecond);
				for (unsigned meshChannelsIndex = 0; meshChannelsIndex < animation->mNumMeshChannels; ++meshChannelsIndex) {
					const aiMeshAnim* meshAnim = animation->mMeshChannels[meshChannelsIndex];
					for (unsigned keyFramesIndex = 0; keyFramesIndex < meshAnim->mNumKeys; ++keyFramesIndex) {
						const aiMeshKey meshKey = meshAnim->mKeys[keyFramesIndex];
					}
				}
				for (unsigned nodeChannelsIndex = 0; nodeChannelsIndex < animation->mNumChannels; ++nodeChannelsIndex) {
					const aiNodeAnim* nodeAnim = animation->mChannels[nodeChannelsIndex];
					NodeAnimation nodeAnimation(nodeAnim->mNodeName.data);
					for (unsigned positionKeysIndex = 0; positionKeysIndex < nodeAnim->mNumPositionKeys; ++positionKeysIndex) {
						const aiVectorKey& positionKey = nodeAnim->mPositionKeys[positionKeysIndex];
						nodeAnimation.addTranslationKey(NodeAnimation::TranslationKey((float)positionKey.mTime, { positionKey.mValue.x, positionKey.mValue.y, positionKey.mValue.z }));
					}
					for (unsigned rotationKeysIndex = 0; rotationKeysIndex < nodeAnim->mNumRotationKeys; ++rotationKeysIndex) {
						const aiQuatKey& quatKey = nodeAnim->mRotationKeys[rotationKeysIndex];
						nodeAnimation.addRotationKey(NodeAnimation::RotationKey((float)quatKey.mTime, quat_cast(quatKey.mValue)));
					}
					for (unsigned scalingKeysIndex = 0; scalingKeysIndex < nodeAnim->mNumScalingKeys; ++scalingKeysIndex) {
						const aiVectorKey& scalingKey = nodeAnim->mScalingKeys[scalingKeysIndex];
						nodeAnimation.addScaleKey(NodeAnimation::ScaleKey((float)scalingKey.mTime, { scalingKey.mValue.x , scalingKey.mValue.y, scalingKey.mValue.z }));
					}
					an.addNodeAnimation(nodeAnimation);
					/*nodeAnim->mPostState;
					nodeAnim->mPreState;*/
				}
				mScene->addAnimation(an);
			}

			//mScene->printAnimationData(filePath.substr(filePath.find_last_of("/\\") + 1, std::string::npos) + ".txt");
		}
		//mScene->printHierachy();
		return true;
	}

	void ModelData::walkThroughChildren(const aiScene* scene, const aiNode* node, Ref<SceneObject>& parent, const glm::mat4& accumulatedTransform)
	{
		Ref<SceneObject> currentParent = nullptr;
		glm::mat4 transform(1.f);

		if (node->mParent == nullptr) {
			// This is the root node
			currentParent = parent;
			currentParent->setName(node->mName.C_Str());
			transform = glm::inverse(mat4_cast(node->mTransformation));
			currentParent->setTransform(transform);
		}
		else {
			transform = mat4_cast(node->mTransformation) * accumulatedTransform;
			currentParent = std::make_shared<SceneObject>(node->mName.C_Str());
			currentParent->setParent(parent);
			currentParent->setTransform(transform);
			parent->addChild(currentParent);
		}

		if (node->mNumChildren > 0) {
			for (unsigned i = 0; i < node->mNumChildren; ++i) {
				walkThroughChildren(scene, node->mChildren[i], currentParent, transform);
			}
		}
		else {
			for (unsigned i = 0; i < node->mNumMeshes; ++i) {
				mMeshesData.emplace_back(processMesh(scene, scene->mMeshes[node->mMeshes[i]]));
			}
		}
	}

	MeshData* ModelData::processMesh(const aiScene* scene, const aiMesh* mesh)
	{
		MeshData::VertexSeparated vs;
		std::unordered_map<Texture2D::Type, std::vector<std::string>> textures;

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
		mComponents = props.Components;

		// Loads the vertex positions values into the vector.
		// While doing it a vertex object is created.
		// So, in order to fill the remaining data (normals, texture coordinates, tangents
		// and bitangent) we just need to access the element and copy the data).
		for (unsigned vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {

			// Positions
			vs.VertexData[MeshData::Positions].push_back(mesh->mVertices[vertexIndex].x);
			vs.VertexData[MeshData::Positions].push_back(mesh->mVertices[vertexIndex].y);
			vs.VertexData[MeshData::Positions].push_back(mesh->mVertices[vertexIndex].z);

			// Gets normal vectors
			if (meshHasNormals) {
				vs.VertexData[MeshData::Normals].push_back(mesh->mNormals[vertexIndex].x);
				vs.VertexData[MeshData::Normals].push_back(mesh->mNormals[vertexIndex].y);
				vs.VertexData[MeshData::Normals].push_back(mesh->mNormals[vertexIndex].z);
			}

			// Gets tangents and bitangents vectors
			if (meshHasTangentsAndBitangents) {
				// Tangents
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mTangents[vertexIndex].x);
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mTangents[vertexIndex].y);
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mTangents[vertexIndex].z);
				// Bitangents
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mBitangents[vertexIndex].x);
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mBitangents[vertexIndex].y);
				vs.VertexData[MeshData::TangentsAndBitangents].push_back(mesh->mBitangents[vertexIndex].z);
			}

			// Gets texture coordinates
			if (meshHasTextureCoordinates) {
				vs.VertexData[MeshData::TextureCoordinates].push_back(mesh->mTextureCoords[0][vertexIndex].x);
				vs.VertexData[MeshData::TextureCoordinates].push_back(mesh->mTextureCoords[0][vertexIndex].y);
			}
		}

		// Process the faces (the primitives that forms a face) a set of several points.
		// Those are the indices.
		if (meshHasFaces) {
			for (unsigned facesIndex = 0; facesIndex < mesh->mNumFaces; facesIndex++) {
				const aiFace& face = mesh->mFaces[facesIndex];
				for (unsigned indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++) {
					vs.Indices.push_back(face.mIndices[indicesIndex]);
				}
			}
		}

		// Gets bones and animations
		if (meshHasBones) {
			for (unsigned bonesIndex = 0; bonesIndex < mesh->mNumBones; ++bonesIndex) {
				const aiBone* bone = mesh->mBones[bonesIndex];
				unsigned boneIndex = 0U;
				if (mBonesMap.find(bone->mName.C_Str()) == mBonesMap.end()) {
					// Bone does not exist: allocate space for a new one
					boneIndex = mBonesOffsets.size();
					mBonesOffsets[boneIndex] = mat4_cast(bone->mOffsetMatrix);
					mBonesMap[bone->mName.C_Str()] = boneIndex;
				}
				else {
					boneIndex = mBonesMap[bone->mName.C_Str()];
				}

				MeshData::Bone newBone(boneIndex);
				for (unsigned boneWeightsIndex = 0; boneWeightsIndex < bone->mNumWeights; ++boneWeightsIndex) {
					vs.AddVertexData(bone->mWeights[boneWeightsIndex].mVertexId, boneIndex, bone->mWeights[boneWeightsIndex].mWeight);
				}
			}

			vs.FixBoneData(mesh->mNumVertices);
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

			bool isMaterialValid = true;
			aiColor4D ambient, diffuse, specular, emissive;
			float shininess = 0.f;
			isMaterialValid = isMaterialValid && (aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient) == aiReturn_SUCCESS);
			isMaterialValid = isMaterialValid && (aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse) == aiReturn_SUCCESS);
			isMaterialValid = isMaterialValid && (aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular) == aiReturn_SUCCESS);
			isMaterialValid = isMaterialValid && (aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive) == aiReturn_SUCCESS);
			isMaterialValid = isMaterialValid && (aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess) == aiReturn_SUCCESS);

			props.mMaterial.Ambient = glm::vec4(ambient.r, ambient.g, ambient.b, ambient.a);
			props.mMaterial.Diffuse = glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a);
			props.mMaterial.Specular = glm::vec4(specular.r, specular.g, specular.b, specular.a);
			props.mMaterial.Emissive = glm::vec4(emissive.r, emissive.g, emissive.b, emissive.a);
			props.mMaterial.Shininess = shininess;

			if (searchForTextures) {
				// 1. ambient maps
				std::vector<std::string>& ambientMap = processMaterial(scene, material, Texture2D::Type::Ambient);
				if (!ambientMap.empty()) {
					textures[Texture2D::Type::Ambient] = ambientMap;
				}

				// 2. diffuse maps 
				std::vector<std::string>& diffuseMap = processMaterial(scene, material, Texture2D::Type::Diffuse);
				if (!diffuseMap.empty()) {
					textures[Texture2D::Type::Diffuse] = diffuseMap;
				}

				// 3. specular maps
				std::vector<std::string>& specularMap = processMaterial(scene, material, Texture2D::Type::Specular);
				if (!specularMap.empty()) {
					textures[Texture2D::Type::Specular] = specularMap;
				}

				// 4. normal maps
				std::vector<std::string>& normalMap = processMaterial(scene, material, Texture2D::Type::Normal);
				if (!normalMap.empty()) {
					textures[Texture2D::Type::Normal] = normalMap;
				}

				// 5. height maps
				std::vector<std::string>& heightMap = processMaterial(scene, material, Texture2D::Type::Height);
				if (!heightMap.empty()) {
					textures[Texture2D::Type::Height] = heightMap;
				}
			}
		}

		// return a meshdata object created from the extracted mesh data
		return new MeshData(props, vs, textures);
	}

	std::vector<std::string> ModelData::processMaterial(const aiScene* scene, const aiMaterial* material, Texture2D::Type textureType)
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
			std::string texPath = texturePath.data;

			// check if texture was loaded before
			auto found = std::find_if(mLoadedTextures.begin(), mLoadedTextures.end(), [&](const Image& image) {
				return (mDirectory + texPath) == image.getPath();
			});

			if (found == mLoadedTextures.end()) {
				// Not loaded yet: load it
				Image image;
				if (texPath[0] == '*') {
					// Texture is embeded in the file
					// Load from scene object
					unsigned index = std::stoi(texPath.substr(1, std::string::npos));
					const aiTexture* imageTexture = scene->mTextures[index];
					if (imageTexture->mHeight != 0) {
						// uncompressed format: load from aiTexels
						// imageTexture->mHeight is the height in pixels
						// imageTexture->mWidth is the width in pixels
						// imageTexture->pcData contains the texels in
						unsigned size = imageTexture->mHeight * imageTexture->mWidth * 4;
						std::vector<unsigned char> pixelData(size);
						unsigned char* begin = &pixelData[0];
						unsigned char* end = begin + size;
						unsigned texelIndex = 0;
						while (begin != end) {
							*begin++ = imageTexture->pcData[texelIndex].r;
							*begin++ = imageTexture->pcData[texelIndex].g;
							*begin++ = imageTexture->pcData[texelIndex].b;
							*begin++ = imageTexture->pcData[texelIndex].a;
							++texelIndex;
						}
						image.create(imageTexture->mWidth, imageTexture->mHeight, pixelData);
					}
					else {
						// compressed format
						// imageTexture->mWidth is the size in bytes of texture
						// imageTexture->achFormatHint contains the extension on the image:
						// imageTexture->achFormatHint is null if there is no information
						// imageTexture->pcData is just a pointer for the data 
					}
					//image.create()
				}
				else {
					image.loadFromFile(mDirectory + texPath);
				}

				mLoadedTextures.emplace_back(image);
			}
			meshTexturesPath.emplace_back(mDirectory + texPath);
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
