
// Ruan C. Keet (2022)
// Log.h

#pragma once

#if defined(MG_DEBUG)
	#include <iostream>
	#include <format>
	
	#define LOG(...) std::cout << "[DEBUG]: " << std::format(__VA_ARGS__) << std::endl
#elif defined(MG_RELEASE)
	#define LOG(...)
#endif