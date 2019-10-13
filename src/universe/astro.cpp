/*
 * astro.cpp
 *
 *  Created on: Apr 4, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/star.h"
#include "universe/astro.h"

using namespace ofs::astro;

// Official gravitational constant [N (m/kg)^2]
constexpr double ofs::astro::G = 6.673889e-11;

vec3d_t ofs::astro::convertEquatorialToEcliptic(double ra, double dec, double pc)
{
	double  theta, phi;
	double  x, y, z;
	vec3d_t rot(J2000Obliquity, 0, 0);
	vec3d_t opos;

	theta = glm::radians(ra) + PI;
	phi   = glm::radians(dec - 90.0);

	opos = vec3d_t(sin(phi)*cos(theta), cos(phi), sin(phi)*-sin(theta));

	return opos * glm::dquat(rot) * pc;
}

//void convertDecimalToDegMinSec(double angle, int &deg, int &min, double &sec)
//{
//
//}
