#pragma once

#include "../Core/Core.h"

#include "Texture.h"

namespace zr
{
	class CubeMap : public Texture
	{
	public:
		/**
		* @brief A class representing a cubemap texture.
		* A series of 6 textures that is arranged in a certain order to mimic the faces of a cube.
		*
		*/
		CubeMap();
		virtual ~CubeMap();

		virtual bool loadFromFiles(const std::string& rightFace, const std::string& leftFace, const std::string& topFace, const std::string& bottomFace, const std::string& backFace, const std::string& frontFace) = 0;

		/**
		*	Faces must be in this order:
		*  	rightFace;
		*		leftFace;
		*		topFace;
		*		bottomFace;
		*		backFace;
		*		frontFace.
		*
		*	If the names of the files containing the face texture is out
		*	of the specified order, then the faces of the rendered cubemap
		*	will be messed up.
		*
		* @param faces An array containeng the paths of the texture to apply to the cube's faces.
		*
		*/
		virtual bool loadFromFiles(const std::vector<std::string>& faces) = 0;

		virtual void setViewProjectionMatrix(const glm::mat4& viewProjectionMatrix) = 0;

		virtual unsigned getHandler() const = 0;

		virtual void render(const glm::mat4& viewProjectionMatrix) = 0;

		static Ref<CubeMap> Create();
	};
}
