/*
* Ruan C. Keet (2022)
* Log.h
*/

#pragma once

#if defined(MG_DEBUG)
	#include <stdio.h>

	#define LOG(...) printf(__VA_ARGS__); printf("\n")
#elif defined(MG_RELEASE)
	#define LOG(...)
#endif