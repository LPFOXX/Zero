#include "zr_pch.h"

#include "Scene.hpp"
#include "Components.hpp"
#include "Entity.hpp"

namespace zr
{
	Scene::Scene() :
		mRegistry(nullptr)
	{
		mRegistry = CreateRef<entt::registry>();
	}

	Entity Scene::CreateEntity()
	{
		Entity entity{ this, mRegistry->create() };
		entity.AttachComponent<TransformComponent>();
		entity.AttachComponent<TagComponent>();
		entity.AttachComponent<IDComponent>(UUID{});
		return entity;
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity{ this, mRegistry->create() };
		entity.AttachComponent<TransformComponent>();
		entity.AttachComponent<TagComponent>(name);
		entity.AttachComponent<IDComponent>(UUID{});
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		mRegistry->destroy(entity.mNativeId);
	}


	void Scene::onUpdate(const Time& elapsedTime)
	{
	}
}
