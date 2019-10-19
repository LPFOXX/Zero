#include <zr_pch.h>

#include "MeshData.h"

namespace zr
{
	MeshData::MeshData(const MeshProperties& meshProps, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<Image>>& textures) :
		mProperties(new MeshProperties(meshProps)),
		mVertices(vertices),
		mIndices(indices),
		mTextures(textures)
	{
	}

	MeshData::~MeshData()
	{

	}

	const std::vector<MeshData::Vertex>& MeshData::getVertices() const
	{
		return mVertices;
	}

	const std::vector<unsigned>& MeshData::getIndices() const
	{
		return mIndices;
	}

	const MeshData::MeshProperties& MeshData::getProperties() const
	{
		return *mProperties;
	}

	const std::unordered_map<Texture2D::Type, std::vector<Image>>& MeshData::getTextures() const
	{
		return mTextures;
	}
}
