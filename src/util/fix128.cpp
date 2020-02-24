/*
 * int128.cpp
 *
 *  Created on: Feb 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"

static constexpr double POW2_31 = 2147483648.0;
static constexpr double POW2_32 = 4294967296.0;
static constexpr double POW2_63 = POW2_32 * POW2_31;
static constexpr double POW2_64 = POW2_32 * POW2_32;

// Fixed point (96.32) format conversion
static constexpr double WORD0_FACTOR = 1.0 / POW2_32;
static constexpr double WORD1_FACTOR = 1.0;
static constexpr double WORD2_FACTOR = POW2_32;
static constexpr double WORD3_FACTOR = POW2_64;

// Convert integer to fixed (96.32)
fix128_t::fix128_t(uint64_t val)
{
	hi = val >> 32;
	lo = val << 32;
}

// Convert double to fixed (64.64)
fix128_t::fix128_t(double val)
{
	// Determine sign bit of value
	bool sign = false;
	if (val < 0) {
		sign = true;
		val = -val;
	}

	double e = floor(val * (1.0 / WORD3_FACTOR));
	if (e < POW2_31) {
		auto w3 = (uint32_t)e;
		val -= w3 * WORD3_FACTOR;
		auto w2 = (uint32_t)(val * (1.0 / WORD2_FACTOR));
		val -= w2 * WORD2_FACTOR;
		auto w1 = (uint32_t)(val * (1.0 / WORD1_FACTOR));
		val -= w1 * WORD1_FACTOR;
		auto w0 = (uint32_t)(val * (1.0 / WORD0_FACTOR));

		hi = ((uint64_t)w3 << 32) | w2;
		lo = ((uint64_t)w1 << 32) | w0;

		if (sign == true)
			negate();
	} else {
		cerr << fmt::sprintf("FIX128: Too big value %lf\n", val);
		hi = 0;
		lo = 0;
	}
}

fix128_t::operator double() const
{
	return 0.0;
}

fix128_t::operator uint64_t() const
{
	return 0;
}
