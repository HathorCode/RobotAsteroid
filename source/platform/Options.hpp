#pragma once
#ifdef _WIN32 || _WIN64
#   include "windows/Options.hpp"
#else
#   error undefined platform
#endif
