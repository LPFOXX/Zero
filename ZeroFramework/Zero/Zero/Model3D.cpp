#include "Model3D.h"
#include "Model3D.h"
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
		mComponents(mModelData->getComponents())
	{
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
		mShader->setUniform("uColor", {.1f, .3f, .8f, 1.f});
		for (auto& m : mMeshes) {
			m->render(mShader);
		}
	}

	void Model3D::update(const Time& elapsedTime)
	{
		//ZR_CORE_INFO("Model3D::update");
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
		std::stringstream vertexShader;
		vertexShader <<R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;	
		)";

		int location = 1;
		if (mComponents & MeshData::Components::Normals) {
			vertexShader << "layout(location = " << location++ << ") in vec3 aNormal;\n";
		}

		if (mComponents & MeshData::Components::TangentsAndBitangents) {
			vertexShader << "layout(location = " << location++ << ") in vec3 aTangent;\n";
			vertexShader << "layout(location = " << location++ << ") in vec3 aBitangent;\n";
		}

		if (mComponents & MeshData::Components::TextureCoordinates) {
			vertexShader << "layout(location = " << location++ << ") in vec2 aTexCoords;\n";
		}

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
			for (unsigned i = 0; i < mMaxAmbientTextures; i++) {
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
			}

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

		std::string vertex = R"(
			#version 330 core
			layout(location = 0) in vec3 aPosition;	
			layout(location = 1) in vec3 aNormal;	
			layout(location = 2) in vec3 aTangent;	
			layout(location = 3) in vec3 aBitangent;	
			layout(location = 4) in vec3 aTexCoords;

			uniform mat4 uViewProjection;

			void main()
			{
				gl_Position = uViewProjection * vec4(aPosition, 1.f);
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
		if (!mShader->loadFromStrings(vertex, fragment)) {
			std::cout << "Can't create model shader object.\n";
		}
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
