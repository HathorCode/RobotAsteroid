#pragma once
#ifdef _WIN32 || _WIN64
#   include "windows/Menu.hpp"
#else
#   error undefined platform
#endif
