
// Ruan C. Keet (2022)
// Log.h

#pragma once

#ifdef MG_DEBUG
#include <iostream>
#define LOG(x) std::cout << "[DEBUG]: " << x << std::endl

#elif MG_RELEASE
#define LOG(x)

#endif