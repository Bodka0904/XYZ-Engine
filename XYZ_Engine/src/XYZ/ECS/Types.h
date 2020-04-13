#pragma once
#include <bitset>

namespace XYZ {
#define MAX_ENTITIES 8192
#define MAX_COMPONENTS 32

	using Entity = unsigned int;
	using Signature = std::bitset<MAX_COMPONENTS>;
	using ComponentType = std::uint8_t;
}