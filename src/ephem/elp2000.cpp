/*
 * elp2000.cpp
 *
 *  Created on: Mar 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/elp2000.h"

using namespace ofs::ephem;

vec3d_t ELP2000Orbit::calculatePosition(double jd) const
{
	double jd19 = jd - 2415020.0;
	double t = jd19 / 36525.0;
	double t2 = t * t;



	return vec3d_t(0, 0, 0);
}



