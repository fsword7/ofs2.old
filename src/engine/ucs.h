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
	vec3d_t v = vec3d_t(0, 0, 0);
	fix128_t x, y, z;
};
