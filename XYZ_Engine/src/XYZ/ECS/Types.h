#pragma once
#include <bitset>

namespace XYZ {
#define MAX_ENTITIES 8192
#define MAX_COMPONENTS 64

	using Entity = unsigned int;
	using Signature = std::bitset<MAX_COMPONENTS>;
	using ComponentType = uint16_t;
}