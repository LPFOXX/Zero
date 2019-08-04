#pragma once

namespace zr
{
	template <typename EventDataType>
	class Observer
	{
	public:
		Observer();
		virtual ~Observer();

		virtual void update(const EventDataType& eventData) = 0;
	};

	template <typename T, typename EventDataType>
	class Observable
	{
	public:
		Observable();
		virtual ~Observable();

		void notify(const EventDataType& eventData);
		void subscribe(std::shared_ptr<Observer<EventDataType>>& observer);
		void unsubscribe(std::shared_ptr<Observer<EventDataType>>& observer);

	private:
		std::vector<std::shared_ptr<Observer<EventDataType>>> mObservers;
	};
}

#include "Observer.inl"
