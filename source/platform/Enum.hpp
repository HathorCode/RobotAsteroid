/*
	Defines portable enums and includes platform specific enums.
*/
#pragma once
#ifdef tcPlatWin
#   include "windows/Enum.hpp"
#else
#   error platform undefined
#endif

namespace tc {
	const int noError = 0;	//Shared enumeration
	enum ShaderInitError {
		/* Anything which returns this could also return noError
		   or a FileError*/
		SIE_failedToCompile = 1,
		SIE_failedToLink,
	};
}
