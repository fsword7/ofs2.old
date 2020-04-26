/*
 * iau.cpp - IAU WGCCRE Rotational Elements package
 *
 *  Created on: Apr 26, 2020
 *      Author: Tim Stark
 *
 *  For more information, check URL links below
 *
 *  https://astrogeology.usgs.gov/search/map/Docs/WGCCRE/WGCCRE2015reprint
 *
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/rotation.h"

using namespace ofs::astro;
using namespace ofs::ephem;

IAURotationModel::IAURotationModel(double poleRA, double poleRARate,
		double poleDec, double poleDecRate,
		double meridian, double period)
: poleRA(poleRA),   poleRARate(poleRARate),
  poleDec(poleDec), poleDecRate(poleDecRate),
  meridianAtEpoch(meridian), rotationRate(period),
  period(abs(360.0 / period))
{
	if (period < 0.0)
		reversal = true;
}

quatd_t IAURotationModel::computeSpin(double tjd) const
{
	tjd -= astro::J2000;

	double spin = ((reversal == true) ? 1 : -1) *
		glm::radians(180.0 + computeMeridian(tjd));
	return yrot(spin);
}

quatd_t IAURotationModel::computeEquatorRotation(double tjd) const
{
	tjd -= astro::J2000;

	vec2d_t pole = computePole(tjd);

	double node        = 90.0 + pole.x; // Right Ascension [RA]
	double inclination = 90.0 - pole.y; // Declination

	if (reversal == false)
		return xrot(glm::radians(-inclination)) * yrot(glm::radians(-node));
	else
		return xrot(PI) * xrot(glm::radians(-inclination)) * yrot(glm::radians(-node));
}

vec2d_t IAURotationModel::computePole(double tjd) const
{
	double t = tjd / 36525.0;
	vec2d_t pole;

	pole.x = poleRA + (poleRARate * tjd);
	pole.y = poleDec + (poleDecRate * tjd);

	return pole;
}

double IAURotationModel::computeMeridian(double tjd) const
{
	return meridianAtEpoch + (rotationRate * tjd);
}
