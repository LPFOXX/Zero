#pragma once

#include "../../vendor/glm/include/glm/glm.hpp"

namespace zr
{
	struct Material
	{
		Material() :
			Ambient(0.f, 0.f, 0.f, 0.f),
			Diffuse(0.f, 0.f, 0.f, 0.f),
			Specular(0.f, 0.f, 0.f, 0.f),
			Emissive(0.f, 0.f, 0.f, 0.f),
			Shininess(0.f)
		{
		}

		Material(const Material& copy) : 
			Ambient(copy.Ambient),
			Diffuse(copy.Diffuse),
			Specular(copy.Specular),
			Emissive(copy.Emissive),
			Shininess(copy.Shininess)
		{

		}

		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		glm::vec4 Emissive;
		float Shininess;
	};

	struct Light
	{
		enum LightType
		{
			None = 0,
			Directional = 1,
			Point = 2,
			Spot = 3
		};

		Light() :
			Ambient(0.f, 0.f, 0.f, 0.f),
			Diffuse(0.f, 0.f, 0.f, 0.f),
			Specular(0.f, 0.f, 0.f, 0.f),
			Type(LightType::None)
		{

		}

		static inline LightType GetType()
		{
			return LightType::None;
		}

		glm::vec4 Ambient;
		glm::vec4 Diffuse;
		glm::vec4 Specular;
		LightType Type;
	};

	struct DirectionalLight : Light
	{
		DirectionalLight() :
			Light(),
			Direction(0.f, 0.f, 0.f)
		{
			Type = Light::LightType::Directional;
		}

		static inline LightType GetType()
		{
			return LightType::Directional;
		}

		glm::vec3 Direction;
	};

	struct PointLight : Light
	{
		PointLight() :
			Light(),
			Position(0.f, 0.f, 0.f),
			AttenuationFactors(0.f, 0.f, 0.f)
		{
			Type = Light::LightType::Point;
		}

		static inline LightType GetType()
		{
			return LightType::Point;
		}

		glm::vec3 Position;
		glm::vec3 AttenuationFactors;
	};
}
