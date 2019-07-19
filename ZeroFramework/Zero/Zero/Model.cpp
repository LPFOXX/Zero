#include <zr_pch.h>

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace zr
{
	Model::Model(const std::string& filePath, unsigned componentsToLoad) :
		mComponents(componentsToLoad)
	{
		Assimp::Importer importer;
		unsigned flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType;
		const aiScene* scene = importer.ReadFile(filePath.c_str(), flags);

		if (!scene) {
			std::ostringstream outStringStream("Error: ASSIMP::IMPORTER:: ");
			outStringStream << importer.GetErrorString();
			throw std::runtime_error(outStringStream.str());
		}

		mDirectory = filePath.substr(0, filePath.find_last_of('/') + 1);

		walkThroughChildren(scene->mRootNode, scene);

		generateShader();
	}

	Model::~Model()
	{

	}

	void Model::setTransformationMatrix(const glm::mat4& transformations)
	{
		mTransformationMatrix = transformations;
	}

	void Model::render(const glm::mat4& viewProjectionMatrix)
	{
		mShader->bind();
		mShader->setUniform("uViewProjection", viewProjectionMatrix);
		mShader->setUniform("uModel", mTransformationMatrix);
		for (const auto& mesh : mMeshes) {
			mesh->render(mShader);
		}
	}

	void Model::walkThroughChildren(const aiNode* rootNode, const aiScene* scene)
	{
		for (unsigned i = 0; i < rootNode->mNumChildren; i++) {
			for (unsigned j = 0; j < rootNode->mChildren[i]->mNumMeshes; j++) {
				mMeshes.emplace_back(processMesh(scene->mMeshes[rootNode->mChildren[i]->mMeshes[j]], scene));
			}

			walkThroughChildren(rootNode->mChildren[i], scene);
		}
	}

	Mesh* Model::processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Mesh::Vertex> vertices;
		std::vector<unsigned> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		bool meshHasNormals = mesh->HasNormals() && (mComponents & Model::LoadComponents::Normals);
		bool meshHasTangentsAndBitangents = mesh->HasTangentsAndBitangents() && (mComponents & Model::LoadComponents::TangentsAndBitangents);
		bool meshHasTextureCoordinates = mesh->HasTextureCoords(0) && (mComponents & Model::LoadComponents::TextureCoordinates);
		bool meshHasFaces = mesh->HasFaces();

		// Loads the vertex positions values into the vector.
		// While doing it a vertex object is created.
		// So, in order to fill the remaining data (normals, texture coordinates, tangents
		// and bitangent) we just need to access the element and copy the data).
		for (unsigned vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			Mesh::Vertex newVertex;

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

		// process materials
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// we assume a convention for sampler names in the shaders. Each diffuse texture should be named
		// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
		// Same applies to other texture as the following list summarizes:
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1. diffuse maps
		std::vector<std::shared_ptr<Texture>> diffuseMaps = processMaterial(material, Texture::TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2. specular maps
		std::vector<std::shared_ptr<Texture>> specularMaps = processMaterial(material, Texture::TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3. normal maps
		std::vector<std::shared_ptr<Texture>> normalMaps = processMaterial(material, Texture::TextureType::Normal);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<std::shared_ptr<Texture>> heightMaps = processMaterial(material, Texture::TextureType::Height);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		mMaxDiffuseTextures = std::max(mMaxDiffuseTextures, static_cast<unsigned>(diffuseMaps.size()));
		mMaxSpecularTextures = std::max(mMaxSpecularTextures, static_cast<unsigned>(specularMaps.size()));
		mMaxNormalTextures = std::max(mMaxNormalTextures, static_cast<unsigned>(normalMaps.size()));
		mMaxHeightTextures = std::max(mMaxHeightTextures, static_cast<unsigned>(heightMaps.size()));

		Mesh::MeshProperties props;
		props.HasNormals = meshHasNormals;
		props.HasTangentsAndBitangents = meshHasTangentsAndBitangents;
		props.HasTextureCoordinates = meshHasTextureCoordinates;
		props.Name = mesh->mName.C_Str();

		// return a mesh object created from the extracted mesh data
		return new Mesh(props, vertices, indices, textures);
	}

	std::vector<std::shared_ptr<Texture>> Model::processMaterial(const aiMaterial* material, Texture::TextureType textureType)
	{
		std::vector<std::shared_ptr<Texture>> textures;

		aiTextureType type;
		switch (textureType) {
			case zr::Texture::None:
			{
				type = aiTextureType_NONE;
				break;
			}
			case zr::Texture::Diffuse:
			{
				type = aiTextureType_DIFFUSE;
				break;
			}
			case zr::Texture::Specular:
			{
				type = aiTextureType_SPECULAR;
				break;
			}
			case zr::Texture::Normal:
			{
				type = aiTextureType_NORMALS;
				break;
			}
			case zr::Texture::Height:
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

		/*aiTextureType_AMBIENT
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
			auto found = std::find_if(mTexturesLoadedPath.begin(), mTexturesLoadedPath.end(), [&texturePath](const std::string& loadedTexturePath) {
				return std::strcmp(loadedTexturePath.c_str(), texturePath.C_Str()) == 0;
			});

			if (found == mTexturesLoadedPath.end()) {
				// Not found: load it
				mTexturesLoadedPath.push_back(texturePath.C_Str());
				textures.emplace_back(Texture::Create(mDirectory + texturePath.C_Str(), textureType));
			}
		}

		return textures;
	}

	void Model::generateShader()
	{
		std::string vertexShader = R"(
			#version 330 core
			layout(location = 0) in vec3 aPos;
			layout(location = 1) in vec3 aNormal;
			layout(location = 2) in vec3 aTangent;
			layout(location = 3) in vec3 aBitangent;
			layout(location = 4) in vec2 aTexCoords;

			out vec2 TexCoords;

			uniform mat4 uModel;
			uniform mat4 uViewProjection;

			void main()
			{
				TexCoords = aTexCoords;
				gl_Position = uViewProjection * uModel * vec4(aPos, 1.0);
			}
		)";

		std::stringstream fragmentShader;
		fragmentShader <<
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec2 TexCoords;\n"
			"\n";


		for (unsigned i = 0; i < mMaxDiffuseTextures; i++) {
			fragmentShader << "uniform sampler2D texture_diffuse" << i + 1 << ";\n";
		}

		for (unsigned i = 0; i < mMaxSpecularTextures; i++) {
			fragmentShader << "uniform sampler2D texture_specular" << i + 1 << ";\n";
		}

		for (unsigned i = 0; i < mMaxNormalTextures; i++) {
			fragmentShader << "uniform sampler2D texture_normal" << i + 1 << ";\n";
		}

		for (unsigned i = 0; i < mMaxHeightTextures; i++) {
			fragmentShader << "uniform sampler2D texture_height" << i + 1 << ";\n";
		}

		fragmentShader <<
			"\n"
			"void main()\n"
			"{\n"
			"	FragColor = texture(texture_diffuse1, TexCoords) * texture(texture_specular1, TexCoords) * texture(texture_height1, TexCoords); \n"
			"}\n";

		// "	FragColor = texture(texture_diffuse1, TexCoords);\n"

		mShader.reset(Shader::Create());
		if (!mShader->loadFromStrings(vertexShader, fragmentShader.str())) {
			std::cout << "Can't create model shader object.\n";
		}
	}
}
