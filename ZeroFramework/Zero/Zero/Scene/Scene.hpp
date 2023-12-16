#pragma once

#include "../Core/Core.h"
#include "../Core/Clock.h"
#include "entt.hpp"

namespace zr
{
	class Entity;
	class Scene
	{
	public:
		Scene();
		virtual ~Scene() = default;

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		void onUpdate(const Time& elapsedTime);

		template <typename... Components, typename Function>
		void Each(const Function&& function) const
		{
			mRegistry->view<Components...>().each(function);
		}

		template <typename... Components, typename Function>
		void Each(const Function&& function)
		{
			mRegistry->view<Components...>().each(function);
		}

		template <typename... Components>
		inline decltype(auto) View() const
		{
			return mRegistry->view<Components...>();
		}

		template <typename... Components>
		inline decltype(auto) View() 
		{
			return mRegistry->view<Components...>();
		}

	private:
		friend class Entity;
		Ref<entt::registry> mRegistry;
	};
}
