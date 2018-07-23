#pragma once
#include <bitset>

namespace ecs{
	namespace Entity {
	using UID = std::bitset<64>;
	using Handle = size_t;
	using Keybits = std::bitset<64>;
	using Lockbits = std::bitset<64>;
	constexpr size_t NULL_ENTITY(0);
	constexpr size_t MAX_ENTITIES{ 2000 };
	}
}
