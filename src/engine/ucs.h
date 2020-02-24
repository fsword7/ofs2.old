/*
 * ucs.h - Universal Coordinate System package
 *
 *  Created on: Feb 23, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <util/fix128.h>

class ucs_t
{
public:
	ucs_t() = default;
	~ucs_t() = default;

public:
//	fix128_t x, y, z;
	int128_t x, y, z;
};
