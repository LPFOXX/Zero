#pragma once

#include <string>
#include <map>

#include "Core.h"
#include "Loadable.hpp"

namespace zr
{
	template <typename T>
	class AssetHandler
	{
	public:
		AssetHandler() :
			mAssetPtr(nullptr),
			mId(0)
		{

		}

		AssetHandler(const Ref<T>& assetPtr, uint32_t assetId) :
			mAssetPtr(assetPtr),
			mId(assetId)
		{

		}

		virtual ~AssetHandler() = default;

		uint32_t getId() const
		{
			return mId;
		}

	private:
		Ref<T> mAssetPtr;
		uint32_t mId;
	};

	template <typename T>
	class AssetManager
	{
	private:
		template <typename U, bool IsAbstract>
		struct AbstractOrNonAbstract
		{
			template <typename... Args>
			static Ref<U> Create(const Args&... args);
		};

		template <typename U>
		struct AbstractOrNonAbstract<U, true>
		{
			// Abstract classes
			template <typename... Args>
			static Ref<U> Create(const Args&... args)
			{
				return T::Create(args...);
			}
		};

		template <typename U>
		struct AbstractOrNonAbstract<U, false>
		{
			// Non-Abstract classes
			template <typename... Args>
			static Ref<U> Create(const Args&... args)
			{
				return CreateRef<U>(args...);
			}
		};

		template <typename U, bool IsLoadable>
		struct LoadOrCreate
		{
			template <typename... Args>
			static Ref<U> Create(const std::string& assetPathOrName, const Args&... args);
		};

		template <typename U>
		struct LoadOrCreate<U, true>
		{
			// Create Loadable class references
			template <typename... Args>
			static Ref<U> Create(const std::string& assetPath, const Args&... args)
			{
				Ref<U>& ref = AbstractOrNonAbstract<U, std::is_abstract<U>::value>::Create();

				if (ref) {
					bool loadSuccess = ref->loadFromFile(assetPath, std::forward<Args>(args)...);
					if (loadSuccess) {
						return ref;
					}
				}

				return nullptr;
			}
		};

		template <typename U>
		struct LoadOrCreate<U, false>
		{
			// Create Non-Loadable class references
			template <typename... Args>
			static Ref<U> Create(const std::string& assetName, const Args&... args)
			{
				return AbstractOrNonAbstract<U, std::is_abstract<U>::value>::Create(args...);
			}
		};

	public:
		AssetManager() :
			mNextId(0)
		{
		}

	public:
		virtual ~AssetManager() = default;

		static constexpr AssetManager<T>& Get()
		{
			static std::unique_ptr<AssetManager<T>> sInstance = std::make_unique<AssetManager<T>>();
			return *sInstance;
		}

		template <typename... Args>
		const std::pair<AssetHandler<T>, bool> load(const std::string& assetPath, const Args&... args)
		{
			return loadOrCreate(assetPath, args...);
		}

		template <typename... Args>
		const std::pair<AssetHandler<T>, bool> create(const std::string& assetName, const Args&... args)
		{
			return loadOrCreate(assetName, args...);
		}

		template <typename... Args>
		const std::pair<AssetHandler<T>, bool> loadOrCreate(const std::string& assetPath, const Args&... args)
		{
			Ref<T>& assetRef = LoadOrCreate<T, std::is_base_of<Loadable, T>::value>::Create(assetPath, args...);

			if (assetRef) {
				uint32_t assetId = getNextId();
				auto& result = mAssets.insert({ assetId, assetRef });
				if (result.second) {
					return { AssetHandler<T>(result.first->second, assetId), true };
				}
			}

			return { AssetHandler<T>(), false };
		}

		/*template <typename... Args>
		const std::pair<AssetHandler<T>, bool> loadOrCreate(const std::string& assetName, const Args&&... args)
		{
			return { AssetHandler<T>(), false };
		}*/

		/*template <typename... Args>
		const std::pair<AssetHandler<T>, bool> create(const std::string& assetName, const Args&&... args)
		{
			Ref<T> toInsert = CreateRef<T>(std::forward<Args>(args)...);
			return { AssetHandler<T>(), false };
		}*/

		template <bool IsAbstract>
		Ref<T> createAssetRef()
		{
			return nullptr;
		}

		template <>
		Ref<T> createAssetRef<true>()
		{
			return AbstractCreator<T>::Create();
		}

		template <>
		Ref<T> createAssetRef<false>()
		{
			return NonAbstractCreator<T>::Create();
		}

		Ref<T> getAsset(const AssetHandler<T>& assetHandler)
		{
			auto& it = mAssets.find(assetHandler.getId());

			if (it != mAssets.end()) {
				return it->second;
			}

			return nullptr;
		}

		/*
		Ref<T> getAsset(uint32_t assetId)
		{
			auto& it = std::find_if(mAssets.begin(), mAssets.end(), [&assetId](const Ref<Asset>& asset) { return asset->getId() == assetId; });

			if (it != mAssets.end()) {
				return it->second;
			}

			return nullptr;
		}*/

	private:
		uint32_t getNextId()
		{
			/// TODO: test this function
			uint32_t firstChecked = (mNextId == std::numeric_limits<uint32_t>::max() ? 1 : ++mNextId);

			while (mAssets.find(mNextId) != mAssets.end()) {
				// Id exists: try next one

				// Check for overflow
				mNextId = (mNextId == std::numeric_limits<uint32_t>::max() ? 1 : ++mNextId);

				if (mNextId == firstChecked) {
					// ran out of ids
					return 0;
				}
			}

			return mNextId;
		}

	private:
		uint32_t mNextId;
		std::map<uint32_t, Ref<T>> mAssets;
	};
}
