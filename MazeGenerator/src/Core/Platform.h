#pragma once

#if defined(_WIN32)
	#if defined(_WIN_32)
		#error "x86 builds not supported"
	#elif defined(_WIN64)
		#define MG_PLATFORM_WINDOWS
	#endif
#elif defined (__linux__)
	#define MG_PLATFORM_LINUX
#elif defined (__APPLE__)
	#error "MacOS not supported"
#endif