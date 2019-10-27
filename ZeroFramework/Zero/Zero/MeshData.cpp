#include <zr_pch.h>

#include "MeshData.h"

namespace zr
{
	MeshData::MeshData(const MeshProperties& meshProps, std::vector<Vertex>& vertices, std::vector<unsigned>& indices, const std::unordered_map<Texture2D::Type, std::vector<std::string>>& textures) :
		mProperties(meshProps),
		mVertices(vertices),
		mIndices(indices),
		mTextures(textures)
	{
	}

	MeshData::MeshData(const MeshProperties& meshProps, const VertexSeparated& vertices, const std::unordered_map<Texture2D::Type, std::vector<std::string>>& textures) :
		mProperties(meshProps),
		mSeparatedVertices(vertices),
		mTextures(textures)
	{
	}

	MeshData::~MeshData()
	{

	}

	std::vector<MeshData::Vertex>& MeshData::getVertices()
	{
		return mVertices;
	}

	MeshData::VertexSeparated& MeshData::getSeparatedVertices()
	{
		return mSeparatedVertices;
	}

	std::vector<unsigned>& MeshData::getIndices()
	{
		return mIndices;
	}

	const MeshData::MeshProperties& MeshData::getProperties() const
	{
		return mProperties;
	}

	const std::unordered_map<Texture2D::Type, std::vector<std::string>>& MeshData::getTextures() const
	{
		return mTextures;
	}
}
