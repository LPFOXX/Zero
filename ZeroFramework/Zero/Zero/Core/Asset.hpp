#pragma once

namespace zr
{
	class Asset
	{
	public:
		Asset() :
			mId(0)
		{	
		}

		virtual ~Asset() = default;

		uint32_t getId() const
		{
			return mId;
		}

	private:
		uint32_t mId;
	};
}
