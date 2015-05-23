/*
	This file just includes the appropriate header for the target platform.
*/
#pragma once
#ifdef _WIN32 || _WIN64
#   include "windows/Win.hpp"
#else
#   error platform undefined
#endif
