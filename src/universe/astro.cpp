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

constexpr double G = 6.673889e-11; // Official gravitational constant [N (m/kg)^2]

vec3d_t ofs::astro::convertEquatorialToEcliptic(double ra, double dec, double pc)
{
	double  theta, phi;
	double  x, y, z;
//	double  km = convertParsecToKilometer(pc);
	vec3d_t rot(-J2000Obliquity, 0, 0);
	vec3d_t opos;

	theta = toRadian(ra) + PI;
	phi   = toRadian(dec - 90.0);

	opos = vec3d_t(sin(phi)*cos(theta), cos(phi), sin(phi)*-sin(theta));

	return glm::normalize(opos * glm::dquat(rot)) * pc;
}