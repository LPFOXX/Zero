#pragma once

#include "../vendor/glm/include/glm/glm.hpp"

#include <map>
#include <vector>

#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Image.h"

#include "ImGui/ImGuiConsole.h"
#include "Core/Log.h"

namespace zr
{
	class MeshData
	{
	public:
		enum Components
		{
			Positions = 1 << 0,
			Normals = 1 << 1,
			TangentsAndBitangents = 1 << 2,
			TextureCoordinates = 1 << 3,
			Textures = 1 << 4,
			Animations = 1 << 5,
			Materials = 1 << 6
		};

		struct MeshProperties
		{
			MeshProperties() :
				Name(""),
				mMaterial(),
				Components(0U)
			{

			}

			std::string Name;
			Material mMaterial;
			unsigned Components;
		};

		struct Vertex
		{
			Vertex() :
				Position(0.f, 0.f, 0.f),
				Normal(0.f, 0.f, 0.f),
				Tangent(0.f, 0.f, 0.f),
				Bitangent(0.f, 0.f, 0.f),
				TextureCoordinates(0.f, 0.f)
			{

			}

			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec3 Tangent;
			glm::vec3 Bitangent;
			glm::vec2 TextureCoordinates;
		};

		struct VertexBoneData
		{
			std::vector<unsigned> BoneIndices;
			std::vector<float> BoneWeights;
		};

		struct Bone
		{
			Bone() :
				Index(),
				Weights()
			{

			}

			Bone(unsigned boneIndex) :
				Index(boneIndex),
				Weights()
			{

			}

			struct Weight
			{
				Weight(unsigned vertexIndex, float weight) :
					VertexIndex(vertexIndex),
					WeightValue(weight)
				{

				}

				unsigned VertexIndex;
				float WeightValue;
			};

			unsigned Index;
			std::vector<Weight> Weights;
		};

		struct BoneInfo
		{
			BoneInfo(unsigned id, float effectOnVertice) :
				BoneId(id),
				BoneEffectOnVertice(effectOnVertice)
			{

			}
			unsigned BoneId;
			float BoneEffectOnVertice;
		};

		struct VertexSeparated
		{
			std::map<MeshData::Components, std::vector<float>> VertexData;
			std::vector<unsigned> Indices;
			std::map<unsigned, VertexBoneData> BoneData;
			unsigned MaxBonesPerVertex = 0U;

			void AddVertexData(unsigned vertexIndex, unsigned boneIndex, float boneWeight)
			{
				BoneData[vertexIndex].BoneIndices.push_back(boneIndex);
				BoneData[vertexIndex].BoneWeights.push_back(boneWeight);

				MaxBonesPerVertex = std::max<unsigned>((unsigned)BoneData[vertexIndex].BoneIndices.size(), MaxBonesPerVertex);
			}

			void FixBoneData(unsigned vertexCount)
			{
				if (BoneData.size() != vertexCount) {
					ZR_CORE_WARN("There are vertex not influenced by any bone");
					ZR_IMGUI_CONSOLE_WARN("There are vertex not affected by any bone");
					for (unsigned i = 0; i < vertexCount; ++i) {
						if (BoneData.find(i) == BoneData.end()) {
							ZR_IMGUI_CONSOLE_INFO("Uninfluenced vertex %d", i);
							AddVertexData(i, 0U, 0.f);
						}
					}

					if (BoneData.size() != vertexCount) {
						ZR_IMGUI_CONSOLE_INFO("Fixed uninfluenced vertices");
					}
				}
			}
		};

	public:
		MeshData(
			const MeshProperties& meshProps,
			std::vector<Vertex>& vertices,
			std::vector<unsigned>& indices,
			const std::unordered_map<Texture2D::Type, std::vector<std::string>>& textures);

		MeshData(
			const MeshProperties& meshProps,
			const VertexSeparated& vertices,
			const std::unordered_map<Texture2D::Type, std::vector<std::string>>& textures
		);
		virtual ~MeshData();

		std::vector<MeshData::Vertex>& getVertices();
		MeshData::VertexSeparated& getSeparatedVertices();
		std::vector<unsigned>& getIndices();
		const MeshData::MeshProperties& getProperties() const;
		const std::unordered_map<Texture2D::Type, std::vector<std::string>>& getTextures() const;

	private:
		std::unordered_map<Texture2D::Type, std::vector<std::string>> mTextures;
		std::vector<MeshData::Vertex> mVertices;
		std::vector<unsigned> mIndices;
		MeshData::MeshProperties mProperties;
		VertexSeparated mSeparatedVertices;
	};
}
