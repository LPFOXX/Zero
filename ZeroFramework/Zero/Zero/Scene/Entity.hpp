#pragma once

#include "entt.hpp"

#include "../Core/Core.h"
#include "../Core/UUID.hpp"
#include "../Scene/Scene.hpp"

namespace zr
{
	class Entity
	{
	public:
		Entity(Scene* scene, entt::entity id);
		virtual ~Entity() = default;

		template <typename ComponentType, typename... Args>
		ComponentType& AttachComponent(Args&&... args)
		{
			return mScene->mRegistry->emplace_or_replace<ComponentType>(mNativeId, std::forward<Args>(args)...);
		}

		template <typename ComponentType>
		void DetachComponent()
		{
			mScene->mRegistry->remove_if_exists<ComponentType>(mNativeId);
		}

		template <typename ComponentType>
		ComponentType& GetComponent()
		{
			return mScene->mRegistry->get<ComponentType>(mNativeId);
		}

		template <typename ComponentType>
		bool HasComponent()
		{
			return mScene->mRegistry->has<ComponentType>(mNativeId);
		}

		operator bool()
		{
			//return mScene->mRegistry->valid(mNativeId);
			return true;
		}

	private:
		friend class Scene;

	private:
		Scene* mScene;
		entt::entity mNativeId;
	};
}
