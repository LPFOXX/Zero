#pragma once

#include <fstream>

#include <assimp/scene.h>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

#include "../vendor/glm/include/glm/gtc/type_ptr.hpp"

#include <SFML/System.hpp>

#include "Core/Core.h"
#include "Core/Log.h"

#include "Image.h"
#include "Renderer/Texture.h"
#include "MeshData.h"

#include "ImGui/ImGuiConsole.h"

#include <sstream>
#include <map>

namespace zr
{
	static inline glm::mat4 mat4_cast(const aiMatrix4x4& from)
	{
		return glm::transpose(glm::make_mat4(&from.a1));
	}

	static inline glm::vec3 vec3_cast(const aiVector3D& from)
	{
		return glm::vec3(from.x, from.y, from.z);
	}

	static inline glm::quat quat_cast(const aiQuaternion& q)
	{
		return glm::quat(q.w, q.x, q.y, q.z);
	}

	class ModelData
	{
	public:
		class NodeAnimation
		{
		public:
			struct TranslationKey
			{
				TranslationKey(float time, const glm::vec3& position) :
					Time(time),
					Position(position)
				{

				}

				float Time;
				glm::vec3 Position;
			};

			struct RotationKey
			{
				RotationKey(float time, const glm::quat& rotation) :
					Time(time),
					Rotation(rotation)
				{

				}

				float Time;
				glm::quat Rotation;
			};

			struct ScaleKey
			{
				ScaleKey(float time, const glm::vec3& scale) :
					Time(time),
					Scale(scale)
				{

				}

				float Time;
				glm::vec3 Scale;
			};

		public:
			NodeAnimation()
			{

			}

			NodeAnimation(const std::string& affectedNode) :
				mAffectedNode(affectedNode)
			{

			}

			virtual ~NodeAnimation()
			{

			}

			const std::string& getNodeName() const
			{
				return mAffectedNode;
			}

			const std::vector<TranslationKey>& getTranslations() const
			{
				return mTranslationKeys;
			}

			const std::vector<RotationKey>& getRotations() const
			{
				return mRotationKeys;
			}

			const std::vector<ScaleKey>& getScales() const
			{
				return mScaleKeys;
			}

			void addTranslationKey(const TranslationKey& translation)
			{
				mTranslationKeys.push_back(translation);
			}

			void addRotationKey(const RotationKey& rotation)
			{
				mRotationKeys.push_back(rotation);
			}

			void addScaleKey(const ScaleKey& scale)
			{
				mScaleKeys.push_back(scale);
			}

		private:
			std::string mAffectedNode;
			std::vector<TranslationKey> mTranslationKeys;
			std::vector<RotationKey> mRotationKeys;
			std::vector<ScaleKey> mScaleKeys;
		};

		class Animation
		{
		public:
			Animation() :
				mName(),
				mDuration(0.f),
				mTicksPerSecond(25.f),
				mNodeAnimations()
			{

			}

			Animation(const std::string& name) :
				mName(name),
				mDuration(0.f),
				mTicksPerSecond(25.f),
				mNodeAnimations()
			{

			}

			virtual ~Animation()
			{

			}

			const std::string& getName() const
			{
				return mName;
			}

			float getDuration() const
			{
				return mDuration;
			}

			float getTicksPerSecond() const
			{
				return mTicksPerSecond;
			}

			const std::map<std::string, NodeAnimation>& getAffectedNodes() const
			{
				return mNodeAnimations;
			}

			void setDuration(double duration, double ticksPerSecond)
			{
				mTicksPerSecond = (float)(ticksPerSecond != 0.0 ? ticksPerSecond : 25.f);
				mDuration = (float)duration / mTicksPerSecond;
			}

			void addNodeAnimation(const NodeAnimation& nodeAnimation)
			{
				mNodeAnimations[nodeAnimation.getNodeName()] = nodeAnimation;
			}

			const NodeAnimation* getNodeAnimation(const std::string& nodeName) const
			{
				auto it = mNodeAnimations.find(nodeName);
				if (it != mNodeAnimations.end()) {
					return &it->second;
				}
				return nullptr;
			}

		private:
			std::string mName;
			float mDuration;
			float mTicksPerSecond;

			std::map<std::string, NodeAnimation> mNodeAnimations;
		};

		class SceneObject
		{
		public:
			SceneObject() = default;
			SceneObject(const std::string& objectName) :
				mName(objectName),
				mParent(nullptr),
				mChildren()
			{

			}

			virtual ~SceneObject()
			{

			}

			void setName(const std::string& name)
			{
				mName = name;
			}

			void setParent(const Ref<SceneObject>& parent)
			{
				mParent = parent;
			}

			void addChild(const Ref<SceneObject>& child)
			{
				mChildren.push_back(child);
			}

			void setTransform(const glm::mat4& transform)
			{
				mTransform = transform;
			}

			const std::string& getName() const
			{
				return mName;
			}

			const glm::mat4& getTransform() const
			{
				return mTransform;
			}

			const std::vector<Ref<SceneObject>>& getChildren() const
			{
				return mChildren;
			}

			void printHierachy(unsigned level) const
			{
				std::stringstream ss;
				for (unsigned i = 0; i < level; ++i) {
					ss << '\t';
				}
				ZR_IMGUI_LOG(ConsoleItem::Trace, "%sNode: %s", ss.str().c_str(), mName.c_str());
				for (auto& child : mChildren) {
					child->printHierachy(level + 1);
				}
			}

		private:
			Ref<SceneObject> mParent;
			std::vector<Ref<SceneObject>> mChildren;
			std::string mName;
			glm::mat4 mTransform = glm::mat4(1.f);
		};

		class Scene
		{
		public:
			Scene() = default;
			Scene(Ref<SceneObject>& rootSceneObject) :
				mRootNode(rootSceneObject)
			{

			}

			virtual ~Scene()
			{

			}

			void addAnimation(const Animation& animation)
			{
				mAnimations.push_back(animation);
			}

			const std::vector<Animation>& getAnimations() const
			{
				return mAnimations;
			}

			const Animation* getAnimation(unsigned animationIndex) const
			{
				if (!mAnimations.empty() && animationIndex <= mAnimations.size() - 1) {
					return &mAnimations[animationIndex];
				}
				
				return nullptr;
			}

			const Animation* getAnimation(const std::string& animationName) const
			{
				auto it = std::find_if(mAnimations.cbegin(), mAnimations.cend(), [&animationName](const Animation& animation) {
					return animation.getName() == animationName;
				});

				if (it != mAnimations.cend()) {
					return &(*it);
				}

				return nullptr;
			}

			int getAnimationIndex(const std::string& animationName) const
			{
				auto it = std::find_if(mAnimations.begin(), mAnimations.end(), [&animationName](const Animation& animation) {
					return animation.getName() == animationName;
				});

				if (it != mAnimations.end()) {
					return (int)std::distance(mAnimations.begin(), it);
				}

				return -1;
			}

			void printHierachy()
			{
				mRootNode->printHierachy(0);
			}

			void printAnimationData(const std::string& fileName) const
			{
				std::ofstream file(fileName);
				for (auto& animation : mAnimations) {
					file << "Animation name: " << animation.getName() << "\n";
					file << "\tDuration in ticks: " << animation.getDuration() << "\n";
					file << "\tTicks per second: " << animation.getTicksPerSecond() << "\n";
					/*file << "\tMesh animation channels count: " << animation->mNumMeshChannels << "\n";
					for (unsigned meshChannelsIndex = 0; meshChannelsIndex < animation->mNumMeshChannels; ++meshChannelsIndex) {
						const aiMeshAnim* meshAnim = animation->mMeshChannels[meshChannelsIndex];
						file << "\t\tMesh affected:" << meshAnim->mName.C_Str() << "\n";
						file << "\t\tKey frames count:" << meshAnim->mNumKeys << "\n";
						for (unsigned keyFramesIndex = 0; keyFramesIndex < meshAnim->mNumKeys; ++keyFramesIndex) {
							const aiMeshKey meshKey = meshAnim->mKeys[keyFramesIndex];
							file << "\t\t\tKey frame time:" << meshKey.mTime << "\n";
							file << "\t\t\tAnim mesh index:" << meshKey.mValue << "\n";
						}
					}*/
					const auto& affectedNodes = animation.getAffectedNodes();
					file << "\tNode animation channels count: " << affectedNodes.size() << "\n";
					for (const auto& node : affectedNodes) {
						file << "\t\tNode affected: " << node.second.getNodeName() << "\n";
						const std::vector<NodeAnimation::TranslationKey>& translationsKeys = node.second.getTranslations();
						const std::vector<NodeAnimation::RotationKey>& rotationKeys = node.second.getRotations();
						const std::vector<NodeAnimation::ScaleKey>& scaleKeys = node.second.getScales();
						file << "\t\tPosition keys count: " << translationsKeys.size() << "\n";
						for (auto& translation : translationsKeys) {
							file << "\t\t\tPosition key time: " << translation.Time << "\n";
							file << "\t\t\tPosition key value: (" << translation.Position.x << "," << translation.Position.y << "," << translation.Position.z << ")\n";
						}
						file << "\t\tRotation keys count: " << rotationKeys.size() << "\n";
						for (auto& rotation : rotationKeys) {
							file << "\t\t\tRotation key time: " << rotation.Time << "\n";
							file << "\t\t\tRotation key value: (" << rotation.Rotation.w << "," << rotation.Rotation.x << "," << rotation.Rotation.y << "," << rotation.Rotation.z << ")\n";
						}
						file << "\t\tScaling keys count: " << scaleKeys.size() << "\n";
						for (auto& scale : scaleKeys) {
							file << "\t\t\tPosition key time: " << scale.Time << "\n";
							file << "\t\t\tPosition key value: (" << scale.Scale.x << "," << scale.Scale.y << "," << scale.Scale.z << ")\n";
						}
					}
				}
				file.close();
			}

			Ref<SceneObject>& getRootNode()
			{
				return mRootNode;
			}

		private:
			Ref<SceneObject> mRootNode;
			std::vector<Animation> mAnimations;
		};

	public:
		ModelData() = default;
		virtual ~ModelData() = default;

		bool loadFromFile(Assimp::Importer& importer, const std::string& filePath, unsigned flags, unsigned& componentsToLoad);

		void walkThroughChildren(const aiScene* scene, const aiNode* node, Ref<SceneObject>& parent, const glm::mat4& accumulatedTransform);

		MeshData* processMesh(const aiScene* scene, const aiMesh* mesh);

		std::vector<std::string> processMaterial(const aiScene* scene, const aiMaterial* material, Texture2D::Type textureType);

		inline const std::vector<Scope<MeshData>>& getMeshesData()
		{
			return mMeshesData;
		}

		inline unsigned getComponents() const
		{
			return mComponents;
		}

		inline const std::vector<Image>& getTextures() const
		{
			return mLoadedTextures;
		}

		inline const Ref<Scene>& getScene() const
		{
			return mScene;
		}

		inline const std::map<std::string, unsigned>& getBoneMap() const
		{
			return mBonesMap;
		}

		inline const std::map<unsigned, glm::mat4>& getBoneOffsets() const
		{
			return mBonesOffsets;
		}

	private:
		void updateFlag(unsigned& flags, unsigned bit, bool hasComponent) const;

	private:
		Ref<Scene> mScene;
		std::vector<Scope<MeshData>> mMeshesData;
		std::vector<Image> mLoadedTextures;
		std::string mDirectory;
		unsigned mComponents;

		std::map<unsigned, glm::mat4> mBonesOffsets;
		std::map<std::string, unsigned> mBonesMap;
	};
}
