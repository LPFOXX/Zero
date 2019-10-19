#include "../zr_pch.h"

namespace zr
{
	template<typename EventDataType>
	inline Observer<EventDataType>::Observer()
	{
	}

	template<typename EventDataType>
	inline Observer<EventDataType>::~Observer()
	{
	}

	template <typename T, typename EventDataType>
	inline Observable<T, EventDataType>::Observable()
	{

	}

	template<typename T, typename EventDataType>
	inline Observable<T, EventDataType>::~Observable()
	{
	}

	template<typename T, typename EventDataType>
	inline void Observable<T, EventDataType>::notify(const EventDataType& eventData)
	{
		for (const auto& observer : mObservers) {
			observer->update(eventData);
		}
	}

	template<typename T, typename EventDataType>
	inline void Observable<T, EventDataType>::subscribe(Ref<Observer<EventDataType>>& observer)
	{
		auto& found = std::find(mObservers.begin(), mObservers.end(), observer);
		if (found == mObservers.end()) {
			ZR_CORE_INFO("[OBSERVABLE] New observer subscribed.");
			mObservers.push_back(observer);
		}
	}

	template<typename T, typename EventDataType>
	inline void Observable<T, EventDataType>::unsubscribe(std::shared_ptr<Observer<EventDataType>>& observer)
	{
		auto& found = std::find(mObservers.begin(), mObservers.end(), observer);
		if (found != mObservers.end()) {
			ZR_CORE_INFO("[OBSERVABLE] Observer unsubscribed.");
			mObservers.erase(found);
		}
	}
}
