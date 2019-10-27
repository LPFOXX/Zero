#pragma once

#include <SFML/System.hpp>

#include "ModelImpl.h"
#include "ModelData.h"
#include "Mesh.h"

namespace zr
{
	class Model3D : public ModelImpl
	{
	public:
		Model3D(const std::string& filePath, Ref<ModelData>& modelData);
		virtual ~Model3D();

		void setTransformationMatrix(const glm::mat4& transformations);
		void setCameraPosition(const glm::vec3& cameraPosition);

		// Inherited via ModelImpl
		virtual void render(const glm::mat4& viewProjectionMatrix) override;
		virtual void update(const Time& elapsedTime) override;
		virtual void setModelTransform(const glm::mat4& modelTranform) override;
		virtual bool setAnimation(const std::string& animationName, bool startAnimation = false) override;
		virtual bool setAnimation(unsigned animationIndex, bool startAnimation = false) override;
		virtual bool getAvailableAnimations(std::vector<std::string>& animationNames) override;
		virtual bool isLoaded() override;
		virtual bool hasAnimations() override;
		
		virtual std::string getShaderLayoutLocations() const;

	private:
		void generateBuffers();
		void generateShader();
		std::string generateVertexShaderUniforms() const;
		void computeBonesTransformations(const Time& elapsedTime);
		void traverseSceneTree(const ModelData::Animation* animation, float animationTime, const Ref<ModelData::SceneObject>& sceneNode, const glm::mat4& parentTransform);
		void computeScaleValues(glm::vec3& scale, float animationTime, const ModelData::NodeAnimation* nodeAnimation);
		void computeRotationValues(glm::quat& rotation, float animationTime, const ModelData::NodeAnimation* nodeAnimation);
		void computeTranslationValues(glm::vec3& translation, float animationTime, const ModelData::NodeAnimation* nodeAnimation);

	private:
		std::string mFilePath;
		Ref<ModelData> mModelData;
		std::vector<Ref<Texture2D>> mTextures;
		Ref<Shader> mShader;
		bool mIsFinishedLoading;

		std::vector<Ref<Mesh>> mMeshes;
		std::string mDirectory;

		glm::mat4 mTransformationMatrix;

		unsigned mComponents = 0;
		int mCurrentAnimation;
		Ref<ModelData::Scene> mModelScene;

		zr::DirectionalLight mDirectionalLight;
		glm::vec3 mCameraPosition = glm::vec3(0.f, 0.f, 0.f);

		std::map<std::string, unsigned> mBoneMap;
		std::map<unsigned, glm::mat4> mBoneOffsets;

		std::vector<glm::mat4> mBoneTransformations;

		Time mAnimationTime;
		bool mIsAnimating = false;
	};
}
