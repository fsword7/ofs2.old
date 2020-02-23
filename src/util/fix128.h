/*
 * fix128.h - 128-bit fixed/integer package
 *
 *  Created on: Feb 23, 2020
 *      Author: Tim Stark
 */

#pragma once

class fix128_t
{
public:
	fix128_t() = default;
	~fix128_t() = default;

public:
	uint64_t hi = 0, lo = 0;
};
