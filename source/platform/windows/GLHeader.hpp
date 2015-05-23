/*
	Includes DirectX or OpenGL headers as appropriate.
*/
#pragma once
#ifdef tcPlatGL
#   include "OGL.hpp"
#else
#   error platform undefined
#endif
