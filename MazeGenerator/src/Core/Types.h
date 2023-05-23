
// Ruan C. Keet (2022)
// Types.h

#pragma once

#include "Platform.h"

namespace maze
{
	typedef unsigned char		byte;
	typedef unsigned short		uint16;
	typedef unsigned int		uint32;
	typedef unsigned long long	uint64;

	typedef char				int8;
	typedef short				int16;
	typedef int					int32;
	typedef long long			int64;

#ifdef MG_PLATFORM_LINUX
	typedef unsigned long long 	size_t;
#endif

}