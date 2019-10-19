//#include "Model.h"
//#include <zr_pch.h>
//
//#include "Core/Log.h"
//#include "Model.h"
//
//namespace zr
//{
//	sf::Mutex ModelLoader::sInstanceMutex;
//	Ref<ModelLoader> ModelLoader::sInstance = nullptr;
//
//	Model3D::Model3D(const std::string& filePath, unsigned componentsToLoad, const ProgressFunction& progressUpdateFunction) :
//		Model(),
//		mModelLoadingThread(&Model3D::loadFromFile, this),
//		mModelData(new ModelData),
//		mProgressFunction(progressUpdateFunction),
//		mFilePath(filePath),
//		mIsLoaded(false),
//		mComponents(componentsToLoad)
//	{
//		mModelLoadingThread.launch();
//		//loadFromFile(ModelLoadingThreadData(filePath, componentsToLoad, progressUpdateFunction));
//		/*Assimp::Importer importer;
//		const aiScene* scene = importer.ReadFile(filePath.c_str(), flags);
//
//		if (!scene) {
//			std::ostringstream outStringStream("Error: ASSIMP::IMPORTER:: ");
//			outStringStream << importer.GetErrorString();
//			throw std::runtime_error(outStringStream.str());
//		}
//
//		mDirectory = filePath.substr(0, filePath.find_last_of('/') + 1);
//		mComponents |= scene->HasTextures() ? MeshData::Textures : 0;*/
//
//		//walkThroughChildren(scene->mRootNode, scene);
//
//		//generateShader();
//
//		mDirectionalLight.Ambient = glm::vec4(.1f, .1f, .1f, 1.f);
//		mDirectionalLight.Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
//		mDirectionalLight.Specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
//		mDirectionalLight.Direction = glm::vec3(0.f, -1.f, 0.f);
//	}
//
//	Model3D::~Model3D()
//	{
//		mModelLoadingThread.wait();
//	}
//
//	void Model3D::setTransformationMatrix(const glm::mat4& transformations)
//	{
//		mTransformationMatrix = transformations;
//	}
//
//	void Model3D::render(const glm::mat4& viewProjectionMatrix)
//	{
//		if (mIsLoaded) {
//			//mModelData->render(viewProjectionMatrix);
//		}
//
//		//mInstance->render(viewProjectionMatrix);
//		/*mShader->bind();
//		mShader->setUniform("uViewProjection", viewProjectionMatrix);
//		mShader->setUniform("uModel", mTransformationMatrix);
//		mShader->setUniform("uLight", mDirectionalLight);
//		mShader->setUniform("uCameraPosition", mCameraPosition);
//		for (const auto& mesh : mMeshes) {
//			mesh->render(mShader);
//		}*/
//	}
//
//	void Model3D::loadFromFile()
//	{
//		Assimp::Importer importer;
//		importer.SetPropertyInteger(AI_CONFIG_PP_RVC_FLAGS, aiComponent_COLORS | aiComponent_LIGHTS | aiComponent_CAMERAS);
//
//		const Ref<ModelProgressHandler>& progressHandler = ModelLoader::Get()->getProgressHandler();
//		if (progressHandler != nullptr) {
//			ModelProgressHandler* newProgressHandler = new ModelProgressHandler(*progressHandler);
//			newProgressHandler->setUpdateFunction(mProgressFunction);
//			importer.SetProgressHandler(dynamic_cast<Assimp::ProgressHandler*>(newProgressHandler));
//			#ifdef ZR_DEBUG_BUILD
//			importer.SetExtraVerbose(true); // debug feature
//			#endif
//		}
//		unsigned flags = (mComponents & MeshData::TangentsAndBitangents ? aiProcess_CalcTangentSpace : 0U) | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_RemoveComponent | aiProcess_SplitLargeMeshes | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials | aiProcess_SortByPType | aiProcess_FindInvalidData | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_FlipUVs;
//		if (!mModelData->loadFromFile(importer, mFilePath, mComponents)) {
//			ZR_CORE_ERROR("[Model] Can't load model from '{0}'", mFilePath);
//		}
//
//		ZR_CORE_INFO("Finished loading model data from '{0}'", mFilePath);
//
//		//mModelDataMap[filePath] = modelData;
//		//aiProcess_RemoveComponent AI_CONFIG_PP_RVC_FLAGS
//		// aiProcess_SplitLargeMeshes AI_CONFIG_PP_SLM_VERTEX_LIMIT (AI_SLM_DEFAULT_MAX_VERTICES) or AI_CONFIG_PP_SLM_TRIANGLE_LIMIT (AI_SLM_DEFAULT_MAX_TRIANGLES)
//		//return std::make_shared<Model3D>(importer, filePath, flags, componentsToLoad);
//		return;
//	}
//
//	void Model3D::generateBuffers()
//	{
//		auto& meshesData = mModelData->getMeshesData();
//
//		for (auto& meshData : meshesData) {
//			meshData->getTextures();
//			//Scope<Mesh> mesh = std::make_unique<Mesh>(meshData->getProperties(), meshData->getVertices(), meshData->getIndices(), meshData->getTextures());
//		}
//	}
//
//	void Model3D::update(const Time& elapsedTime)
//	{
//		if (mModelData->isFinishedLoading()) {
//			generateBuffers();
//		}
//	}
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
//	void Model3D::generateShader()
//	{
//		std::string vertexShader = R"(
//			#version 330 core
//			layout(location = 0) in vec3 aPos;
//			layout(location = 1) in vec3 aNormal;
//			layout(location = 2) in vec3 aTangent;
//			layout(location = 3) in vec3 aBitangent;
//			layout(location = 4) in vec2 aTexCoords;
//
//			out vec2 TexCoords;
//			out vec3 Normal;
//			out vec4 FragmentPosition;
//
//			uniform mat4 uModel;
//			uniform mat4 uViewProjection;
//
//			void main()
//			{
//				TexCoords = aTexCoords;
//				Normal = aNormal;
//				FragmentPosition = uModel * vec4(aPos, 1.0);
//				gl_Position = uViewProjection * uModel * vec4(aPos, 1.0);
//			}
//		)";
//
//		std::stringstream fragmentShader;
//		fragmentShader << R"(
//			#version 330 core
//			#define LIGHT_TYPE_DIRECTIONAL	1
//			#define LIGHT_TYPE_POINT		2
//			#define LIGHT_TYPE_SPOT			3
//
//			out vec4 FragColor;
//			in vec2 TexCoords;	// only needed when using textures
//			in vec3 Normal;
//			in vec4 FragmentPosition;
//		)";
//
//		if (mComponents & MeshData::Textures) {
//			for (unsigned i = 0; i < mMaxAmbientTextures; i++) {
//				fragmentShader << "uniform sampler2D texture_ambient" << i + 1 << ";\n";
//			}
//
//			for (unsigned i = 0; i < mMaxDiffuseTextures; i++) {
//				fragmentShader << "uniform sampler2D texture_diffuse" << i + 1 << ";\n";
//			}
//
//			for (unsigned i = 0; i < mMaxSpecularTextures; i++) {
//				fragmentShader << "uniform sampler2D texture_specular" << i + 1 << ";\n";
//			}
//
//			for (unsigned i = 0; i < mMaxNormalTextures; i++) {
//				fragmentShader << "uniform sampler2D texture_normal" << i + 1 << ";\n";
//			}
//
//			for (unsigned i = 0; i < mMaxHeightTextures; i++) {
//				fragmentShader << "uniform sampler2D texture_height" << i + 1 << ";\n";
//			}
//
//			fragmentShader <<
//				"\n"
//				"void main()\n"
//				"{\n"
//				"	//FragColor = texture(texture_ambient1, TexCoords) * texture(texture_diffuse1, TexCoords) * texture(texture_specular1, TexCoords) * texture(texture_height1, TexCoords); \n"
//				"	FragColor = texture(texture_ambient1, TexCoords); \n"
//				"	//FragColor = texture(texture_diffuse1, TexCoords); \n"
//				"}\n";
//		}
//		else {
//			fragmentShader << R"(
//				struct Material {
//					vec4 Ambient;
//					vec4 Diffuse;
//					vec4 Specular;
//					vec4 Emissive;
//					float Shininess;
//				};
//
//				struct Light {
//					vec4 Ambient;
//					vec4 Diffuse;
//					vec4 Specular;
//					int Type;
//					vec3 Direction;				// For directional and spot lights
//
//					vec3 Position;				// For point and spot lights 
//					vec3 AttenuationFactors;	// For point and spot lights
//					vec2 CutOffValues;			// For spot lights (component 0: inner, component 1: outter)
//				};
//
//				vec4 computeDirectionalLightImpact(Material inMaterial, Light inLight, vec3 inNormal, vec3 inViewDirection)
//				{
//					vec3 lightDirection = normalize(-inLight.Direction);
//	
//					// diffuse shading
//					float diffuseFactor = max(dot(inNormal, lightDirection), 0.f);
//	
//					// specular shading
//					vec3 reflectedLightDirection = reflect(-lightDirection, inNormal);
//					float specularFactor = pow(max(dot(inViewDirection, reflectedLightDirection), 0.f), inMaterial.Shininess);
//	
//					vec4 diffuse = inLight.Diffuse   * diffuseFactor;
//					vec4 specular = inLight.Specular * specularFactor;
//	
//					return (inLight.Ambient + diffuse + specular) * inMaterial.Diffuse;
//				}
//
//				vec4 computePointLightImpact(Material inMaterial, Light inLight, vec3 inNormal, vec3 inViewDirection)
//				{
//					vec3 lightDirection = normalize(inLight.Position - FragmentPosition.xyz);
//	
//					// diffuse shading
//					float diffuseFactor = max(dot(inNormal, lightDirection), 0.f);
//	
//					// specular shading
//					vec3 reflectedLightDirection = reflect(-lightDirection, inNormal);
//					float specularFactor = pow(max(dot(inViewDirection, reflectedLightDirection), 0.f), inMaterial.Shininess);
//	
//					// attenuation
//					float distance = length(inLight.Position - FragmentPosition.xyz);
//					float attenuation = 1.f / (inLight.AttenuationFactors.x + inLight.AttenuationFactors.y * distance + inLight.AttenuationFactors.z * distance * distance);
//	
//					// combine results
//					vec4 diffuse = inLight.Diffuse * diffuseFactor;
//					vec4 specular = inLight.Specular * specularFactor;
//	
//					return (inLight.Ambient + diffuse + specular) * inMaterial.Diffuse * attenuation;
//				}
//
//				uniform Material uMaterial;
//				uniform Light uLight;
//				uniform vec3 uCameraPosition;
//			)";
//		}
//
//		fragmentShader << R"(
//
//			void main()
//			{
//				//FragColor = texture(texture_ambient1, TexCoords) * texture(texture_diffuse1, TexCoords) * texture(texture_specular1, TexCoords) * texture(texture_height1, TexCoords);
//				//FragColor = uMaterial.Diffuse;
//				//FragColor = texture(texture_diffuse1, TexCoords);
//
//				vec3 normalizedNormal = normalize(Normal);
//				vec3 viewDirection = normalize(uCameraPosition - FragmentPosition.xyz);
//
//				if(uLight.Type == LIGHT_TYPE_DIRECTIONAL)
//				{
//					FragColor = computeDirectionalLightImpact(uMaterial, uLight, normalizedNormal, viewDirection); 
//				}
//			}
//		)";
//			
//
//		ZR_CORE_TRACE("Generated Vertex Shader\n{0}", vertexShader);
//		ZR_CORE_TRACE("Generated Fragment Shader\n{0}", fragmentShader.str());
//
//		
//
//		// "	FragColor = texture(texture_diffuse1, TexCoords);\n"
//
//		mShader.reset(Shader::Create());
//		if (!mShader->loadFromStrings(vertexShader, fragmentShader.str())) {
//			std::cout << "Can't create model shader object.\n";
//		}
//	}
//
//	ModelLogger::~ModelLogger()
//	{
//
//	}
//
//	bool ModelLogger::attachStream(Assimp::LogStream* pStream, unsigned int severity)
//	{
//		return false;
//	}
//
//	bool ModelLogger::detatchStream(Assimp::LogStream* pStream, unsigned int severity)
//	{
//		return false;
//	}
//
//	void ModelLogger::OnDebug(const char* message)
//	{
//		ZR_CORE_TRACE("[ASSIMP] {0}", message);
//	}
//
//	void ModelLogger::OnInfo(const char* message)
//	{
//		ZR_CORE_INFO("[ASSIMP] {0}", message);
//	}
//
//	void ModelLogger::OnWarn(const char* message)
//	{
//		ZR_CORE_WARN("[ASSIMP] {0}", message);
//	}
//
//	void ModelLogger::OnError(const char* message)
//	{
//		ZR_CORE_ERROR("[ASSIMP] {0}", message);
//	}
//
//	ModelProgressHandler::ModelProgressHandler() :
//		Assimp::ProgressHandler(),
//		zr::ProgressHandler()
//	{
//
//	}
//
//	ModelProgressHandler::~ModelProgressHandler()
//	{
//	}
//
//	bool ModelProgressHandler::Update(float percentage)
//	{
//		ZR_CORE_INFO("Model loading progress: {0:.2f}%", percentage * 100.f);
//		notify(percentage);
//		if (mProgressFunction) {
//			mProgressFunction(percentage);
//		}
//		
//		mProgress = percentage;
//		return true;
//	}
//}
