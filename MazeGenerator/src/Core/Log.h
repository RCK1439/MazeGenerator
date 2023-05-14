#pragma once

#ifdef MG_DEBUG
#include <iostream>
#define LOG(x) std::cout << "[DEBUG]: " << x << std::endl

#elif MG_RELEASE
#define LOG(x)

#endif