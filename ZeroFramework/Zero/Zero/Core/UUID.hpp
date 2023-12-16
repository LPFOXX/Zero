#pragma once

#include <random>

namespace zr
{
	class UUID final
	{
	public:
		UUID()
		{
			mUUID = sDistribution(sRandomEngine);
		}

		inline operator uint64_t() const { return mUUID; }
		
	private:
		UUID(uint64_t init) : mUUID{ init } {}

	public:
		static inline UUID Zero() { return UUID{ 0 }; }

	private:
		inline static std::mt19937_64 sRandomEngine{ std::random_device{}() };
		inline static std::uniform_int_distribution<uint64_t> sDistribution{};
	private:
		uint64_t mUUID;
	};
}
