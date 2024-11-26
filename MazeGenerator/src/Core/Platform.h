#pragma once

#if defined(_WIN32)
#	if defined(_WIN64)
#		define MG_PLATFORM_WINDOWS
#       define _AMD64_
#	else
#		error "x86 builds not supported"
#	endif
#elif defined(__linux__)
#	define MG_PLATFORM_LINUX
#elif defined(__APPLE__)
#	error "MacOS not supported"
#endif