#pragma once

namespace ecs {
	constexpr size_t MAX_COMPONENTS{ 32 };
	using ComponentType = size_t;

	namespace Internal {
		inline const ComponentType getUniqueComponentID() noexcept
		{
			static ComponentType lastID{ 0u };
			return lastID++;
		}
	}

	template <typename C>
	inline const ComponentType getComponentTypeID() noexcept
	{
		static_assert(std::is_base_of<Component, C>::value, "C must inherit from Component\n");

		static ComponentType typeID{ Internal::getUniqueComponentID() };
		return typeID;
	}

	struct Component {

	};
}
