#pragma once
#include <memory>
#include <bitset>

namespace ecs {
	using EventType = size_t;
	constexpr size_t MAX_EVENT_TYPES = 64;

	namespace Internal {
		inline const EventType getUniqueEventID() noexcept
		{
			static EventType lastID{ 0u };
			return lastID++;
		}
	}

	template <typename E>
	inline const EventType getEventTypeID() noexcept
	{
		static_assert(std::is_base_of<Event, E>::value, "E must inherit from Event\n");

		static EventType typeID{ Internal::getUniqueEventID() };
		return typeID;
	}

	template<typename E>
	struct IEvent {
		IEvent() {
			static_assert(std::is_base_of<Event, E>::value, "E must inherit from Event\n");
			tag = getEventTypeID<E>();
		}
		size_t	tag;
	};

	struct Event : public IEvent<Event> {
		Event() {
		}
		typedef std::shared_ptr<Event> Ptr;
		typedef std::bitset<MAX_EVENT_TYPES> Signature;
	};
}