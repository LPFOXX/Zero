//#include <zr_pch.h>
//
//#include "Core/Log.h"
//
//#include <assimp/Importer.hpp>
//
//#include "ModelData.h"
//
//namespace zr
//{
//	bool ModelData::loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned componentsToLoad)
//	{
//		mComponents = componentsToLoad;
//
//		const aiScene* scene = importer.ReadFile(filePath.c_str(), flags);
//		if (!scene) {
//			ZR_CORE_ERROR("[ASSIMP] {0}", importer.GetErrorString());
//			return false;
//		}
//
//		mDirectory = filePath.substr(0, filePath.find_last_of('/') + 1);
//		mComponents |= scene->HasTextures() ? MeshData::Textures : 0;
//
//		walkThroughChildren(scene->mRootNode, scene);
//
//		// Finished loading
//		sf::Lock lock(mObjectMutex);
//		mIsFinishedLoading = true;
//		ZR_CORE_INFO("[MODELDATA] Model loading has finished.");
//	}
//
//	void ModelData::walkThroughChildren(const aiNode* rootNode, const aiScene* scene)
//	{
//		if (rootNode->mNumChildren != 0) {
//			for (unsigned i = 0; i < rootNode->mNumChildren; i++) {
//				for (unsigned j = 0; j < rootNode->mChildren[i]->mNumMeshes; j++) {
//					mMeshesData.emplace_back(processMesh(scene->mMeshes[rootNode->mChildren[i]->mMeshes[j]], scene));
//				}
//
//				walkThroughChildren(rootNode->mChildren[i], scene);
//			}
//		}
//		else {
//			for (unsigned i = 0; i < rootNode->mNumMeshes; i++) {
//				mMeshesData.emplace_back(processMesh(scene->mMeshes[rootNode->mMeshes[i]], scene));
//			}
//		}
//	}
//
//	MeshData* ModelData::processMesh(const aiMesh* mesh, const aiScene* scene)
//	{
//		std::vector<MeshData::Vertex> vertices;
//		std::vector<unsigned> indices;
//		std::unordered_map<Texture2D::Type, std::vector<Image>> textures;
//
//		bool meshHasNormals = mesh->HasNormals() && (mComponents & MeshData::Normals);
//		bool meshHasTangentsAndBitangents = mesh->HasTangentsAndBitangents() && (mComponents & MeshData::TangentsAndBitangents);
//		bool meshHasTextureCoordinates = mesh->HasTextureCoords(0) && (mComponents & MeshData::TextureCoordinates);
//		bool meshHasFaces = mesh->HasFaces();
//
//		// Loads the vertex positions values into the vector.
//		// While doing it a vertex object is created.
//		// So, in order to fill the remaining data (normals, texture coordinates, tangents
//		// and bitangent) we just need to access the element and copy the data).
//		for (unsigned vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
//			MeshData::Vertex newVertex;
//
//			// Positions
//			newVertex.Position.x = mesh->mVertices[vertexIndex].x;
//			newVertex.Position.y = mesh->mVertices[vertexIndex].y;
//			newVertex.Position.z = mesh->mVertices[vertexIndex].z;
//
//			// Gets normal vectors
//			if (meshHasNormals) {
//				newVertex.Normal.x = mesh->mNormals[vertexIndex].x;
//				newVertex.Normal.y = mesh->mNormals[vertexIndex].y;
//				newVertex.Normal.z = mesh->mNormals[vertexIndex].z;
//			}
//
//			// Gets tangents and bitangents vectors
//			if (meshHasTangentsAndBitangents) {
//				// Tangents
//				newVertex.Tangent.x = mesh->mTangents[vertexIndex].x;
//				newVertex.Tangent.y = mesh->mTangents[vertexIndex].y;
//				newVertex.Tangent.z = mesh->mTangents[vertexIndex].z;
//				// Bitangents
//				newVertex.Bitangent.x = mesh->mBitangents[vertexIndex].x;
//				newVertex.Bitangent.y = mesh->mBitangents[vertexIndex].y;
//				newVertex.Bitangent.z = mesh->mBitangents[vertexIndex].z;
//			}
//
//			// Gets texture coordinates
//			if (meshHasTextureCoordinates) {
//				newVertex.TextureCoordinates.x = mesh->mTextureCoords[0][vertexIndex].x;
//				newVertex.TextureCoordinates.y = mesh->mTextureCoords[0][vertexIndex].y;
//			}
//
//			vertices.push_back(newVertex);
//		}
//
//		// Process the faces (the primitives that forms a face) a set of several points.
//		// Those are the indices.
//		if (meshHasFaces) {
//			for (unsigned facesIndex = 0; facesIndex < mesh->mNumFaces; facesIndex++) {
//				const aiFace& face = mesh->mFaces[facesIndex];
//
//				for (unsigned indicesIndex = 0; indicesIndex < face.mNumIndices; indicesIndex++) {
//					indices.push_back(face.mIndices[indicesIndex]);
//				}
//			}
//		}
//
//		// process materials
//		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
//		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
//		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
//		// Same applies to other texture as the following list summarizes:
//		// diffuse: texture_diffuseN
//		// specular: texture_specularN
//		// normal: texture_normalN
//
//		aiColor4D ambient, diffuse, specular, emissive;
//		float shininess = 0.f;
//		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient);
//		aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
//		aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
//		aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emissive);
//		aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);
//
//		MeshData::MeshProperties props;
//		props.mMaterial.Ambient.r = ambient.r;
//		props.mMaterial.Ambient.g = ambient.g;
//		props.mMaterial.Ambient.b = ambient.b;
//		props.mMaterial.Ambient.a = ambient.a;
//
//		props.mMaterial.Diffuse.r = diffuse.r;
//		props.mMaterial.Diffuse.g = diffuse.g;
//		props.mMaterial.Diffuse.b = diffuse.b;
//		props.mMaterial.Diffuse.a = diffuse.a;
//
//		props.mMaterial.Specular.r = specular.r;
//		props.mMaterial.Specular.g = specular.g;
//		props.mMaterial.Specular.b = specular.b;
//		props.mMaterial.Specular.a = specular.a;
//
//		props.mMaterial.Emissive.r = emissive.r;
//		props.mMaterial.Emissive.g = emissive.g;
//		props.mMaterial.Emissive.b = emissive.b;
//		props.mMaterial.Emissive.a = emissive.a;
//
//		props.mMaterial.Shininess = shininess;
//
//		// 1. ambient maps
//		textures[Texture2D::Type::Ambient] = processMaterial(material, Texture2D::Type::Ambient);
//		// 2. diffuse maps 
//		textures[Texture2D::Type::Diffuse] = processMaterial(material, Texture2D::Type::Diffuse);
//		// 3. specular maps
//		textures[Texture2D::Type::Specular] = processMaterial(material, Texture2D::Type::Specular);
//		// 4. normal maps 
//		textures[Texture2D::Type::Normal] = processMaterial(material, Texture2D::Type::Normal);
//		// 5. height maps
//		textures[Texture2D::Type::Height] = processMaterial(material, Texture2D::Type::Height);
//
//		props.Components |= meshHasNormals ? MeshData::Normals : 0U;
//		props.Components |= meshHasTangentsAndBitangents ? MeshData::TangentsAndBitangents : 0U;
//		props.Components |= meshHasTextureCoordinates ? MeshData::TextureCoordinates : 0U;
//		props.Components |= textures.size() != 0U ? MeshData::Textures : 0U;
//
//		props.Name = mesh->mName.C_Str();
//
//		// return a meshdata object created from the extracted mesh data
//		return new MeshData(props, vertices, indices, textures);
//	}
//
//	std::vector<Image> ModelData::processMaterial(const aiMaterial* material, Texture2D::Type textureType)
//	{
//		std::vector<Image> textures;
//
//		aiTextureType type;
//		switch (textureType) {
//			case zr::Texture2D::Type::Ambient:
//			{
//				type = aiTextureType_AMBIENT;
//				break;
//			}
//			case zr::Texture2D::Type::None:
//			{
//				type = aiTextureType_NONE;
//				break;
//			}
//			case zr::Texture2D::Type::Diffuse:
//			{
//				type = aiTextureType_DIFFUSE;
//				break;
//			}
//			case zr::Texture2D::Type::Specular:
//			{
//				type = aiTextureType_SPECULAR;
//				break;
//			}
//			case zr::Texture2D::Type::Normal:
//			{
//				type = aiTextureType_NORMALS;
//				break;
//			}
//			case zr::Texture2D::Type::Height:
//			{
//				type = aiTextureType_HEIGHT;
//				break;
//			}
//			default:
//			{
//				type = aiTextureType_UNKNOWN;
//				break;
//			}
//		}
//		/*
//		aiTextureType_EMISSIVE
//		aiTextureType_SHININESS
//		aiTextureType_OPACITY
//		aiTextureType_DISPLACEMENT
//		aiTextureType_LIGHTMAP
//		aiTextureType_REFLECTION*/
//
//
//		unsigned materialCount = material->GetTextureCount(type);
//
//		for (unsigned i = 0; i < materialCount; i++) {
//			aiString texturePath;
//			material->GetTexture(type, i, &texturePath);
//
//			// check if texture was loaded before
//			auto found = std::find_if(mTexturesLoadedPath.begin(), mTexturesLoadedPath.end(), [&texturePath](const std::string& loadedTexturePath) {
//				return std::strcmp(loadedTexturePath.c_str(), texturePath.C_Str()) == 0;
//			});
//
//			if (found == mTexturesLoadedPath.end()) {
//				//// Not found: load it
//				//mTexturesLoadedPath.push_back(texturePath.C_Str());
//				//textures.emplace_back(Image::Create(mDirectory + texturePath.C_Str(), textureType));
//
//				Image image;
//				image.loadFromFile(mDirectory + texturePath.C_Str());
//				textures.emplace_back(image);
//			}
//		}
//
//		return textures;
//	}
//}
