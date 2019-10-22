#pragma once

#include "../vendor/glm/include/glm/glm.hpp"

#include "Renderer/Texture.h"
#include "Renderer/Material.h"
#include "Image.h"

namespace zr
{
	class MeshData
	{
	public:
		enum Components
		{
			Normals					= 1 << 0,
			TangentsAndBitangents	= 1 << 1,
			TextureCoordinates		= 1 << 2,
			Textures				= 1 << 3,
			Animations				= 1 << 4,
			Materials				= 1 << 5
		};

		struct MeshProperties
		{
			enum BitField : unsigned char
			{
				Textures = 1 << 0,
				Normals = 1 << 1,
				TangentsAndBitangents = 1 << 2,
				TextureCoordinates = 1 << 3
			};

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

		struct Bone
		{
			Bone() :
				Name(),
				Weights()
			{

			}

			Bone(const std::string& boneName) :
				Name(boneName),
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

			std::string Name;
			std::vector<Weight> Weights;
			glm::mat4 OffsetMatrix;
		};

	public:
		MeshData(const MeshProperties& meshProps, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<std::string>>& textures);
		virtual ~MeshData();

		std::vector<MeshData::Vertex>& getVertices();
		std::vector<unsigned>& getIndices();
		const MeshData::MeshProperties& getProperties() const;
		const std::unordered_map<Texture2D::Type, std::vector<std::string>>& getTextures() const;

	private:
		std::unordered_map<Texture2D::Type, std::vector<std::string>> mTextures;
		std::vector<MeshData::Vertex> mVertices;
		std::vector<unsigned> mIndices;
		Ref<MeshData::MeshProperties> mProperties;
	};
}
