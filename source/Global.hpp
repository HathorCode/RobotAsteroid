/*
	Defines global typedefs.
*/
#pragma once
#include <cstdint>
#include <cstring>
#include <cassert>

#ifdef _DEBUG
	const bool debug = true;    //so we can use nicer if''s when applicable
#else
	const bool debug = false;
#endif

typedef signed char int8;
typedef unsigned char uint8;
typedef std::int16_t int16;
typedef std::uint16_t uint16;
typedef std::int32_t int32;
typedef std::uint32_t uint32;
typedef std::int64_t int64;
typedef std::uint64_t uint64;
typedef uint8 bool8;
typedef uint32 bool32;
typedef float float32;
typedef double float64;
#if tcPlat64
	typedef int64 intptr;
	typedef uint64 uintptr;
#else
	typedef int32 intptr;
	typedef uint32 uintptr;
#endif

namespace robitRabit {
	//Simple string with stored length
	struct ConstStr {
		uint32 len;
		char const* str;
		ConstStr(const char* initStr) {
			len = (uint32)strlen(initStr);
			str = initStr;
		}
		ConstStr() {}
	};
	struct Str {
		uint32 len;
		char* str;
		Str(char* initStr) {
			len = (uint32)strlen(initStr);
			str = initStr;
		}
		Str() {}
	};
}
