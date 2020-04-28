/*
 * iau-wgccre.cpp - IAU WGCCRE Rotational Elements package
 *
 *  Created on: Apr 26, 2020
 *      Author: Tim Stark
 *
 *  WGCCRE provides some papers for IAU rotational elements below.
 *
 *  https://astrogeology.usgs.gov/groups/IAU-WGCCRE
 *
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/rotation.h"

using namespace ofs::astro;
using namespace ofs::ephem;

// ******** IAU Rotational Model ********

quatd_t IAURotationalModel::computeSpin(double tjd) const
{
	tjd -= astro::J2000;

	double spin = ((reversal == true) ? -1 : 1) *
		glm::radians(180.0 + computeMeridian(tjd));
	return yrot(spin);
}

quatd_t IAURotationalModel::computeEquatorRotation(double tjd) const
{
	tjd -= astro::J2000;

	vec2d_t pole = computePole(tjd);

	double node        = pole.x + 90.0; // Right Ascension [RA]
	double inclination = 90.0 - pole.y; // Declination

	if (reversal == false)
		return xrot(PI) * xrot(glm::radians(-inclination)) * yrot(glm::radians(-node));
	else
		return xrot(glm::radians(-inclination)) * yrot(glm::radians(-node));
}

// ******** Precession Rotational Model ********

class IAUPrecessionRotationalModel : public IAURotationalModel
{
public:
	IAUPrecessionRotationalModel(double poleRA, double poleRARate,
			double poleDec, double poleDecRate,
			double meridian, double rate)
	: IAURotationalModel(abs(360.0 / rate)),
	  poleRA(poleRA),   poleRARate(poleRARate),
	  poleDec(poleDec), poleDecRate(poleDecRate),
	  meridianAtEpoch(meridian), rotationRate(rate)
	{
		if (rate < 0.0)
			reversal = true;
	}

	vec2d_t computePole(double tjd) const
	{
		double t = tjd / 36525.0;
		vec2d_t pole;

		pole.x = poleRA + (poleRARate * tjd);
		pole.y = poleDec + (poleDecRate * tjd);

		return pole;
	}

	double computeMeridian(double tjd) const
	{
		return meridianAtEpoch + (rotationRate * tjd);
	}

private:
	double poleRA, poleRARate;
	double poleDec, poleDecRate;
	double meridianAtEpoch;
	double rotationRate;
};

// ******** Neptune Rotational Model ********

class IAUNeptuneRotationalModel : public IAURotationalModel
{
public:
	IAUNeptuneRotationalModel()
	: IAURotationalModel(360.0 / 536.3128492)
	{}

	vec2d_t computePole(double d) const
	{
		vec2d_t pole;
		double  T = d / 36525.0;
		double  N = glm::radians(357.85 + 52.316 * T);

		pole.x = 299.36 + (0.70 * sin(N));
		pole.y =  43.46 - (0.51 * cos(N));

		return pole;
	}

	double computeMeridian(double d) const
	{
		double  T = d / 36525.0;
		double  N = glm::radians(357.85 + 52.316 * T);

		return 253.18 + (536.3128492 * d) - (0.48 * sin(N));
	}
};

// ********************************************************

RotationModel *RotationModel::create(const string &name)
{
	if (name == "p03lp-earth")
		return new EarthRotationModel();

	if (name == "iau-sun")
		return new IAUPrecessionRotationalModel(
			286.13,     0.0,
			 63.87,     0.0,
			 84.176,   14.1844000);

	if (name == "iau-mercury")
		return new IAUPrecessionRotationalModel(
			281.0097,  -0.0328,
			 61.4143,  -0.0049,
			329.5469,   6.1385025);

	if (name == "iau-venus")
		return new IAUPrecessionRotationalModel(
			272.16,     0.0,
			 67.16,     0.0,
			160.20,    -1.4813688);

	if (name == "iau-earth")
		return new IAUPrecessionRotationalModel(
			  0.0,     -0.641,
			 90.0,     -0.557,
			190.147,  360.9856235);

	if (name == "iau-mars")
		return new IAUPrecessionRotationalModel(
			317.68143, -0.1061,
			 52.88650, -0.0609,
			176.630,  350.8919822);

	if (name == "iau-saturn")
		return new IAUPrecessionRotationalModel(
			 40.589,   -0.036,
			 83.537,   -0.004,
			 38.90,  810.7939024);

	if (name == "iau-uranus")
		return new IAUPrecessionRotationalModel(
			257.311,    0.0,
		    -15.175,    0.0,
			203.81,  -501.1600928);

	if (name == "iau-neptune")
		return new IAUNeptuneRotationalModel();

	return nullptr;
}