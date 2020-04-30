/*
 * elp-mpp02.cpp - ELP/MPP02 Lunar ephemeral data
 *
 *  Created on: Mar 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/elp-mpp02.h"
#include "universe/astro.h"

using namespace ofs::ephem;

ELP2000Orbit::ELP2000Orbit(dataMode mode)
{
	setupParameters(mode);
}

void ELP2000Orbit::setupParameters(dataMode mode)
{

	switch (mode)
	{
	case elpUseLLR:
		break;
	case elpUseDE406:
		break;
	}

}

vec3d_t ELP2000Orbit::calculatePosition(double jd) const
{
	// Julian time since EPOCH J2000.0
	double t = (jd - 2451545.0) / 36525.0;

	return vec3d_t(0, 0, 0);
}



