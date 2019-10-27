#include <zr_pch.h>

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/ProgressHandler.hpp>

#include "Zero/Core/Core.h"
#include "ModelLoader.h"
#include "Model3D.h"

namespace zr
{
	Model3D::Model3D(const std::string& filePath, Ref<ModelData>& modelData) :
		ModelImpl(),
		mFilePath(filePath),
		mModelData(modelData),
		mShader(nullptr),
		mTransformationMatrix(glm::mat4(1.f)),
		mComponents(mModelData->getComponents()),
		mCurrentAnimation(-1),
		mModelScene(mModelData->getScene())
	{
		mBoneMap = modelData->getBoneMap();
		mBoneOffsets = modelData->getBoneOffsets();
		mBoneTransformations.resize(mBoneMap.size(), glm::mat4(1.f));
		mBoneTransformations.shrink_to_fit();

		generateBuffers();
		generateShader();

		mModelData.reset();
	}

	Model3D::~Model3D()
	{
	}

	void Model3D::setTransformationMatrix(const glm::mat4& transformations)
	{
		mTransformationMatrix = transformations;
	}

	void Model3D::setCameraPosition(const glm::vec3& cameraPosition)
	{
		mCameraPosition = cameraPosition;
	}

	void Model3D::render(const glm::mat4& viewProjectionMatrix)
	{
		//ZR_CORE_INFO("Model3D::render");
		mShader->bind();
		mShader->setUniform("uViewProjection", viewProjectionMatrix);
		mShader->setUniform("uTranformMatrix", mTransformationMatrix);
		mShader->setUniform("uColor", { .1f, .3f, .8f, 1.f });
		for (auto& m : mMeshes) {
			m->render(mShader);
		}
	}

	void Model3D::update(const Time& elapsedTime)
	{
		//ZR_CORE_INFO("Model3D::update");
		computeBonesTransformations(elapsedTime);
	}

	void Model3D::setModelTransform(const glm::mat4& modelTranform)
	{
		mTransformationMatrix = modelTranform;
	}

	bool Model3D::setAnimation(const std::string& animationName)
	{
		return false;
	}

	bool Model3D::setAnimation(unsigned animationIndex)
	{
		return false;
	}

	bool Model3D::getAvailableAnimations(std::vector<std::string>& animations)
	{
		return false;
	}

	std::string Model3D::getShaderLayoutLocations() const
	{
		auto& it = std::max_element(mMeshes.begin(), mMeshes.end(), [](const Ref<Mesh>& a, const Ref<Mesh>& b) {
			return a->getVertexArrayMaxLayoutIndex() < b->getVertexArrayMaxLayoutIndex();
		});

		return (*it)->getShaderLayoutLocation();
	}

	void Model3D::generateBuffers()
	{
		auto& meshesData = mModelData->getMeshesData();
		auto& textures = mModelData->getTextures();

		for (auto& texture : textures) {
			Ref<Texture2D> t = Texture2D::Create();
			t->loadFromImage(texture);
			t->setSmooth(true);
			t->generateMipMaps();

			mTextures.emplace_back(t);
		}

		for (auto& meshData : meshesData) {
			mMeshes.emplace_back(new Mesh(*meshData, mTextures));
		}
	}

	void Model3D::generateShader()
	{
		ZR_IMGUI_CONSOLE_INFO("Shader\n%s", getShaderLayoutLocations().c_str());
		std::stringstream vertexShader;
		vertexShader << "#version 330 core\n";
		vertexShader << getShaderLayoutLocations();

		vertexShader << R"(
			out vec2 vTexCoords;
			out vec3 vNormal;
			out vec4 vFragmentPosition;

			uniform mat4 uModel;
			uniform mat4 uViewProjection;

			void main()
			{
				vTexCoords = aTexCoords;
				vNormal = aNormal;
				vFragmentPosition = uModel * vec4(aPosition, 1.0);
				gl_Position = uViewProjection * uModel * vec4(aPosition, 1.0);
			}
		)";

		std::stringstream fragmentShader;
		fragmentShader << R"(
			#version 330 core
			#define LIGHT_TYPE_DIRECTIONAL	1
			#define LIGHT_TYPE_POINT		2
			#define LIGHT_TYPE_SPOT			3
			
			out vec4 FragColor;
			in vec2 vTexCoords;	// only needed when using textures
			in vec3 vNormal;
			in vec4 vFragmentPosition;
		)";

		if (mComponents & MeshData::Textures) {
			/*for (unsigned i = 0; i < mMaxAmbientTextures; i++) {
				fragmentShader << "uniform sampler2D texture_ambient" << i + 1 << ";\n";
			}

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
			}*/

			fragmentShader <<
				"\n"
				"void main()\n"
				"{\n"
				"	//FragColor = texture(texture_ambient1, vTexCoords) * texture(texture_diffuse1, vTexCoords) * texture(texture_specular1, vTexCoords) * texture(texture_height1, vTexCoords); \n"
				"	FragColor = texture(texture_ambient1, vTexCoords); \n"
				"	//FragColor = texture(texture_diffuse1, vTexCoords); \n"
				"}\n";
		}
		else {
			fragmentShader << R"(
				struct Material {
					vec4 Ambient;
					vec4 Diffuse;
					vec4 Specular;
					vec4 Emissive;
					float Shininess;
				};
			
				struct Light {
					vec4 Ambient;
					vec4 Diffuse;
					vec4 Specular;
					int Type;
					vec3 Direction;				// For directional and spot lights
			
					vec3 Position;				// For point and spot lights 
					vec3 AttenuationFactors;	// For point and spot lights
					vec2 CutOffValues;			// For spot lights (component 0: inner, component 1: outter)
				};
			
				vec4 computeDirectionalLightImpact(Material inMaterial, Light inLight, vec3 inNormal, vec3 inViewDirection)
				{
					vec3 lightDirection = normalize(-inLight.Direction);
				
					// diffuse shading
					float diffuseFactor = max(dot(inNormal, lightDirection), 0.f);
				
					// specular shading
					vec3 reflectedLightDirection = reflect(-lightDirection, inNormal);
					float specularFactor = pow(max(dot(inViewDirection, reflectedLightDirection), 0.f), inMaterial.Shininess);
				
					vec4 diffuse = inLight.Diffuse   * diffuseFactor;
					vec4 specular = inLight.Specular * specularFactor;
				
					return (inLight.Ambient + diffuse + specular) * inMaterial.Diffuse;
				}
			
				vec4 computePointLightImpact(Material inMaterial, Light inLight, vec3 inNormal, vec3 inViewDirection)
				{
					vec3 lightDirection = normalize(inLight.Position - vFragmentPosition.xyz);
				
					// diffuse shading
					float diffuseFactor = max(dot(inNormal, lightDirection), 0.f);
				
					// specular shading
					vec3 reflectedLightDirection = reflect(-lightDirection, inNormal);
					float specularFactor = pow(max(dot(inViewDirection, reflectedLightDirection), 0.f), inMaterial.Shininess);
				
					// attenuation
					float distance = length(inLight.Position - vFragmentPosition.xyz);
					float attenuation = 1.f / (inLight.AttenuationFactors.x + inLight.AttenuationFactors.y * distance + inLight.AttenuationFactors.z * distance * distance);
				
					// combine results
					vec4 diffuse = inLight.Diffuse * diffuseFactor;
					vec4 specular = inLight.Specular * specularFactor;
				
					return (inLight.Ambient + diffuse + specular) * inMaterial.Diffuse * attenuation;
				}
			
				uniform Material uMaterial;
				uniform Light uLight;
				uniform vec3 uCameraPosition;
			)";
		}

		fragmentShader << R"(
			
			void main()
			{
				//FragColor = texture(texture_ambient1, vTexCoords) * texture(texture_diffuse1, vTexCoords) * texture(texture_specular1, vTexCoords) * texture(texture_height1, vTexCoords);
				//FragColor = uMaterial.Diffuse;
				//FragColor = texture(texture_diffuse1, vTexCoords);
			
				vec3 normalizedNormal = normalize(vNormal);
				vec3 viewDirection = normalize(uCameraPosition - vFragmentPosition.xyz);
			
				if(uLight.Type == LIGHT_TYPE_DIRECTIONAL)
				{
					FragColor = computeDirectionalLightImpact(uMaterial, uLight, normalizedNormal, viewDirection); 
				}
			}
		)";

		// ZR_CORE_TRACE("Generated Vertex Shader\n{0}", vertexShader.str());
		// ZR_CORE_TRACE("Generated Fragment Shader\n{0}", fragmentShader.str());

		std::stringstream vertex;
		vertex << "#version 330 core\n";
		vertex << getShaderLayoutLocations() << "\n";
		vertex << generateVertexShaderUniforms();
		vertex << R"(
			void main()
			{
				gl_Position = uViewProjection * uTranformMatrix * vec4(aPosition, 1.f);
			}
		)";
		std::string fragment = R"(
			#version 330 core
			
			out vec4 vColor;
			uniform vec4 uColor;

			void main()
			{
				vColor = uColor;		
			}
		)";

		mShader = Shader::Create();
		if (!mShader->loadFromStrings(vertex.str(), fragment)) {
			std::cout << "Can't create model shader object.\n";
		}
	}

	std::string Model3D::generateVertexShaderUniforms() const
	{
		std::stringstream ss;
		ss << "uniform mat4 uViewProjection;\n";
		ss << "uniform mat4 uTranformMatrix;\n";
		return ss.str();
	}

	void Model3D::computeBonesTransformations(const Time& elapsedTime)
	{
		if (!isAnimating || mCurrentAnimation == -1) {
			return;
		}

		const ModelData::Animation* animation = mModelScene->getAnimation(mCurrentAnimation);

		mAnimationTime += elapsedTime;
		float timeInTicks = mAnimationTime.asSeconds() * animation->getTicksPerSecond();
		float animationTime = fmod(timeInTicks, animation->getDuration());

		traverseSceneTree(animation, animationTime, mModelScene->getRootNode(), glm::mat4(1.f));
	}

	void Model3D::traverseSceneTree(const ModelData::Animation* animation, float animationTime, const Ref<ModelData::SceneObject>& sceneNode, const glm::mat4& parentTransform)
	{
		const std::string& nodeName = sceneNode->getName();
		const ModelData::NodeAnimation* nodeAnimation = animation->getNodeAnimation(nodeName);
		glm::mat4 nodeTransformation = sceneNode->getTransform();
		if (nodeAnimation != nullptr) {
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			computeScaleValues(scale, animationTime, nodeAnimation);
			computeRotationValues(rotation, animationTime, nodeAnimation);
			computeTranslationValues(translation, animationTime, nodeAnimation);

			nodeTransformation = glm::translate(glm::mat4(1.f), translation) * glm::mat4(rotation) * glm::scale(glm::mat4(1.f), scale);
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransformation;

		if (mBoneMap.find(nodeName) != mBoneMap.end()) {
			unsigned boneIndex = mBoneMap[nodeName];
			mBoneTransformations[boneIndex] = mModelScene->getRootNode()->getTransform() * globalTransformation * mBoneOffsets[boneIndex];
		}

		const auto& sceneNodeChildren = sceneNode->getChildren();
		for (const auto& child : sceneNodeChildren) {
			traverseSceneTree(animation, animationTime, child, globalTransformation);
		}
	}

	void Model3D::computeScaleValues(glm::vec3& scale, float animationTime, const ModelData::NodeAnimation* nodeAnimation)
	{
		const std::vector<ModelData::NodeAnimation::ScaleKey>& scaleKeys = nodeAnimation->getScales();
		if (scaleKeys.size() == 1) {
			scale = scaleKeys[0].Scale;
			return;
		}

		/*std::find_if(scaleKeys.begin(), scaleKeys.end(), [&animationTime](const ModelData::NodeAnimation::ScaleKey& sk) {
			return sk.Time > animationTime;
		});*/

		unsigned index = 0;
		unsigned next = 0;
		for (unsigned i = 0; i < scaleKeys.size() - 1; i++) {
			if (animationTime < scaleKeys[i + 1].Time) {
				index = i;
				next = index + 1;
				break;
			}
		}

		float deltaTime = scaleKeys[next].Time - scaleKeys[index].Time;
		float factor = (animationTime - scaleKeys[index].Time) / deltaTime;
		factor = clamp(0.f, 1.f, factor);
		glm::vec3& delta = scaleKeys[next].Scale - scaleKeys[index].Scale;
		scale = scaleKeys[index].Scale + factor * delta;
	}

	void Model3D::computeRotationValues(glm::quat& rotation, float animationTime, const ModelData::NodeAnimation* nodeAnimation)
	{
		const std::vector<ModelData::NodeAnimation::RotationKey>& rotationKeys = nodeAnimation->getRotations();
		if (rotationKeys.size() == 1) {
			rotation = rotationKeys[0].Rotation;
			return;
		}

		/*std::find_if(scaleKeys.begin(), scaleKeys.end(), [&animationTime](const ModelData::NodeAnimation::ScaleKey& sk) {
			return sk.Time > animationTime;
		});*/

		unsigned index = 0;
		unsigned next = 0;
		for (unsigned i = 0; i < rotationKeys.size() - 1; i++) {
			if (animationTime < rotationKeys[i + 1].Time) {
				index = i;
				next = index + 1;
				break;
			}
		}

		float deltaTime = rotationKeys[next].Time - rotationKeys[index].Time;
		float factor = (animationTime - rotationKeys[index].Time) / deltaTime;
		factor = clamp(0.f, 1.f, factor);
		const glm::quat& end = rotationKeys[next].Rotation;
		const glm::quat& start = rotationKeys[index].Rotation;
		rotation = glm::lerp(start, end, factor);
		rotation = glm::normalize(rotation);
	}

	void Model3D::computeTranslationValues(glm::vec3& translation, float animationTime, const ModelData::NodeAnimation* nodeAnimation)
	{
		const std::vector<ModelData::NodeAnimation::TranslationKey>& translationKeys = nodeAnimation->getTranslations();
		if (translationKeys.size() == 1) {
			translation = translationKeys[0].Position;
			return;
		}

		/*std::find_if(scaleKeys.begin(), scaleKeys.end(), [&animationTime](const ModelData::NodeAnimation::ScaleKey& sk) {
			return sk.Time > animationTime;
		});*/

		unsigned index = 0;
		unsigned next = 0;
		for (unsigned i = 0; i < translationKeys.size() - 1; i++) {
			if (animationTime < translationKeys[i + 1].Time) {
				index = i;
				next = index + 1;
				break;
			}
		}

		float deltaTime = translationKeys[next].Time - translationKeys[index].Time;
		float factor = (animationTime - translationKeys[index].Time) / deltaTime;
		factor = clamp(0.f, 1.f, factor);
		glm::vec3& delta = translationKeys[next].Position - translationKeys[index].Position;
		translation = translationKeys[index].Position + factor * delta;
	}
}


//Model3D::Model3D(const std::string& filePath, unsigned componentsToLoad, const ProgressFunction& progressUpdateFunction) :
//	
//	mModelData(new ModelData),
//	mProgressFunction(progressUpdateFunction),
//	mFilePath(filePath),
//	mIsLoaded(false),
//	mComponents(componentsToLoad)
//{
//	//loadFromFile(ModelLoadingThreadData(filePath, componentsToLoad, progressUpdateFunction));
//	/*Assimp::Importer importer;
//	const aiScene* scene = importer.ReadFile(filePath.c_str(), flags);
//
//	if (!scene) {
//		std::ostringstream outStringStream("Error: ASSIMP::IMPORTER:: ");
//		outStringStream << importer.GetErrorString();
//		throw std::runtime_error(outStringStream.str());
//	}
//
//	mDirectory = filePath.substr(0, filePath.find_last_of('/') + 1);
//	mComponents |= scene->HasTextures() ? MeshData::Textures : 0;*/
//
//	//walkThroughChildren(scene->mRootNode, scene);
//
//	//generateShader();
//
//	mDirectionalLight.Ambient = glm::vec4(.1f, .1f, .1f, 1.f);
//	mDirectionalLight.Diffuse = glm::vec4(1.f, 1.f, 1.f, 1.f);
//	mDirectionalLight.Specular = glm::vec4(1.f, 1.f, 1.f, 1.f);
//	mDirectionalLight.Direction = glm::vec3(0.f, -1.f, 0.f);
//}
