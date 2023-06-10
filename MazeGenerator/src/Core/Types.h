
// Ruan C. Keet (2022)
// Types.h

#pragma once

#include "Platform.h"

namespace maze
{
	typedef unsigned char		u8;
	typedef unsigned short		u16;
	typedef unsigned int		u32;
	typedef unsigned long long	u64;

	typedef signed char			s8;
	typedef signed short		s16;
	typedef signed int			s32;
	typedef signed long long	s64;

	typedef float				f32;
	typedef long double			f64;

#ifdef MG_PLATFORM_LINUX
	typedef unsigned long long 	size_t;
#endif

}