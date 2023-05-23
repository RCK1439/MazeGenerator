
// Ruan C. Keet (2022)
// Log.h

#pragma once

#if defined(MG_DEBUG)
	#if defined(_MSC_VER)
		#include <iostream>
		#include <format>

		#define LOG(...) std::cout << "[DEBUG]: " << std::format(__VA_ARGS__) << std::endl
	#elif defined(__GNUC__)
		#include <stdio.h>

		#define LOG(...) printf(__VA_ARGS__); printf("\n")
	#endif
#elif defined(MG_RELEASE)
	#define LOG(...)
#endif