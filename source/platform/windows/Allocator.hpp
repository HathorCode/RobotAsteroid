/*
	Allocates memory for the game on a stack allocator.
	This will be used for storing files and data for the levels.
*/
#pragma once
#include "Platform.hpp"

#include <cstdio>

namespace robitRabit {
	uint8* Alloc(intptr amount) {
		assert(amount > 0);
		return (uint8*)malloc(amount);
	}
	uint8* Realloc(uint8* bytes, intptr newAmount) {
		assert(newAmount > 0);
		return (uint8*)::realloc((void*)bytes, newAmount);
	}
	void Free(uint8* bytes) {
		::free((void*)bytes);
	}
}