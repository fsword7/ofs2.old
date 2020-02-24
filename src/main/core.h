/*
 * core.h - OFS core header package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <filesystem>
#include <climits>

#include <fmt/printf.h>

using namespace std;

namespace fs = std::filesystem;

typedef __int128_t int128_t;
typedef __uint128_t uint128_t;

#include "util/color.h"
#include "main/math.h"
#include "engine/ucs.h"

#define APP_FULL_NAME	"Orbital Flight Simulator"
#define APP_SHORT_NAME	"OFS"

// Default window scene size
#define OFS_DEFAULT_WIDTH	1600
#define OFS_DEFAULT_HEIGHT	1000
//#define OFS_DEFAULT_WIDTH	1400
//#define OFS_DEFAULT_HEIGHT	750
#define OFS_DEFAULT_FOV		35.0


// Four CC values for magic codes
inline uint32_t FOURCC(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
    return ( (uint32_t) (((d)<<24) | (uint32_t(c)<<16) | (uint32_t(b)<<8) | uint32_t(a)) );
}

inline uint32_t getFourCC(const char *code)
{
    return ((uint32_t)code[3] << 24) |
           ((uint32_t)code[2] << 16) |
           ((uint32_t)code[1] << 8)  |
           ((uint32_t)code[0]);
}

inline char *strFourCC(uint32_t code)
{
	static char str[5];

	str[0] = char(code >> 24);
	str[1] = char(code >> 16);
	str[2] = char(code >> 8);
	str[3] = char(code);
	str[4] = '\n';

	return str;
}

#define checkAllFlags(flags, mask) ((flags & (mask)) == (mask))
#define checkAnyFlags(flags, mask) (flags & (mask))
