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

	private:
		void generateBuffers();
		void generateShader();

	private:
		std::string mFilePath;
		Ref<ModelData> mModelData;
		std::vector<Ref<Texture2D>> mTextures;
		Ref<Shader> mShader;

		std::vector<Ref<Mesh>> mMeshes;
		std::string mDirectory;

		glm::mat4 mTransformationMatrix;

		unsigned mMaxAmbientTextures = 0;
		unsigned mMaxDiffuseTextures = 0;
		unsigned mMaxSpecularTextures = 0;
		unsigned mMaxNormalTextures = 0;
		unsigned mMaxHeightTextures = 0;

		unsigned mComponents = 0;

		zr::DirectionalLight mDirectionalLight;
		glm::vec3 mCameraPosition = glm::vec3(0.f, 0.f, 0.f);
	};
}
