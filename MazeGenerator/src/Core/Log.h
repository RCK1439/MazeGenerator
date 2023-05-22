
// Ruan C. Keet (2022)
// Log.h

#pragma once

#if defined(MG_DEBUG)
	#include <iostream>

	#if defined(WIN32)
		#include <format>
		#define LOG(...) std::cout << "[DEBUG]: " << std::format(__VA_ARGS__) << std::endl
	#else
		#define LOG(...) std::cout << "[DEBUG]: Undefined" << std::endl
	#endif
	
#elif defined(MG_RELEASE)
	#define LOG(...)
#endif