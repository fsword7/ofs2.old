/*
 * vsop87.cpp
 *
 *  Created on: Mar 18, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/vsop87.h"

using namespace ofs::ephem;

#define VSOP_SERIES(series) vsop87s_t(series, ARRAY_SIZE(series))

#include "ephem/vsop87mer.cpp" // Mercury
#include "ephem/vsop87ven.cpp" // Venus
#include "ephem/vsop87ear.cpp" // Earth
#include "ephem/vsop87mar.cpp" // Mars
#include "ephem/vsop87jup.cpp" // Jupiter
#include "ephem/vsop87sat.cpp" // Saturn
#include "ephem/vsop87ura.cpp" // Uranus
#include "ephem/vsop87nep.cpp" // Neptune

VSOP87Orbit::VSOP87Orbit(vsop87s_t *sL, int nL,
						 vsop87s_t *sB, int nB,
						 vsop87s_t *sR, int nR,
						 double period)
: sL(sL), nL(nL),
  sB(sB), nB(nB),
  sR(sR), nR(nR),
  period(period),
  boundingRadius(0.0)
{
}

double VSOP87Orbit::sum(const vsop87s_t &series, double t) const
{
	if (series.nTerms < 1)
		return 0.0;

	double x = 0.0;
	vsop87_t *terms = &series.terms[0];

	for (int idx = 0; idx < series.nTerms; idx++, terms++)
		x += terms->a * cos(terms->b + terms->c * t);

	return x;
}

vec3d_t VSOP87Orbit::calculatePosition(double jd) const
{
	// Heliocenteric coordinate system
	double l = 0.0; // Longitude
	double b = 0.0; // Latitude
	double r = 0.0; // Radius

	// Julian time since J2000.0
	double t = (jd - 2451545.0) / 365250.0;
	double T;

	// Evaluate series for L (longitude)
	for (int idx = 0, T = 1.0; idx < nL; idx++) {
		l += sum(sL[idx], t) * T;
		T = t * T;
	}

	// Evaluate series for B (latitude)
	for (int idx = 0, T = 1.0; idx < nB; idx++) {
		b += sum(sB[idx], t) * T;
		T = t * T;
	}

	// Evaluate series for R (radius)
	for (int idx = 0, T = 1.0; idx < nR; idx++) {
		r += sum(sR[idx], t) * T;
		T = t * T;
	}

//	fmt::fprintf(cout, "VSOP87 - JD %lf => T %lf P %lf, %lf, %lf\n",
//		jd, t, glm::degrees(l), glm::degrees(b), r);
//	fmt::fprintf(cout, "XYZ (%lf, %lf, %lf)\n",
//		sin(b)*cos(l), cos(b), sin(b)*-sin(l));

	// Convert AU to Kilometer
	r *= KM_PER_AU;
	l += PI;
	b -= PI / 2.0;

	// Astrocentric coordinate system
	return vec3d_t(sin(b)*cos(l), cos(b), sin(b)*-sin(l)) * r;
}

vec3d_t VSOP87Orbit::calculateVelocity(double jd) const
{
	return vec3d_t(0, 0, 0);
}

Orbit *VSOP87Orbit::create(const string &name)
{
	if (name == "vsop87-earth") {
		Orbit *orbit = new VSOP87Orbit( earth_L, 5,
										earth_B, 5,
										earth_R, 5,
										365.25);
		return orbit;
	}

	return nullptr;
}
