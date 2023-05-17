
// Ruan C. Keet (2022)
// Log.h

#pragma once

#ifdef MG_DEBUG
#include <iostream>
#include <format>
#define LOG(...) std::cout << "[DEBUG]: " << std::format(__VA_ARGS__) << std::endl

#elif MG_RELEASE
#define LOG(x)

#endif