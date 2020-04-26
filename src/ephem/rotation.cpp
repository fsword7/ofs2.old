/*
 * rotation.cpp - OFS Rotation Model package
 *
 *  Created on: Oct 8, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/rotation.h"
#include "ephem/precession.h"

using namespace ofs::ephem;

quatd_t RotationModel::getEquatorRotation(double tjd) const
{
	return quatd_t(1, 0, 0, 0);
}

vec3d_t RotationModel::getAngularVelocity(double tjd) const
{
	return vec3d_t(0, 0, 0);
}

quatd_t RotationModel::getRotation(double tjd) const
{
	return spin(tjd) * getEquatorRotation(tjd);
}

// ******** Caching Rotation Model ********

quatd_t CachingRotationModel::spin(double jd) const
{
	if (jd != lastTime) {
		lastSpin      = computeSpin(jd);
		lastTime      = jd;
		validSpin     = true;
		validEquator  = false;
		validVelocity = false;
	} else if(validSpin == false) {
		lastSpin      = computeSpin(jd);
		validSpin     = true;
	}

	return lastSpin;
}

quatd_t CachingRotationModel::getEquatorRotation(double jd) const
{
	if (jd != lastTime) {
		lastEquator   = computeEquatorRotation(jd);
		lastTime      = jd;
		validSpin     = false;
		validEquator  = true;
		validVelocity = false;
	} else if(validEquator == false) {
		lastEquator   = computeEquatorRotation(jd);
		validEquator  = true;
	}

	return lastEquator;
}

vec3d_t CachingRotationModel::getAngularVelocity(double jd) const
{
	if (jd != lastTime) {
		lastVelocity  = computeAngularVelocity(jd);
		lastTime      = jd;
		validSpin     = false;
		validEquator  = false;
		validVelocity = true;
	} else if(validVelocity == false) {
		lastVelocity  = computeAngularVelocity(jd);
		validVelocity = true;
	}

	return lastVelocity;
}

vec3d_t CachingRotationModel::computeAngularVelocity(double tjd) const
{
	return vec3d_t(0, 0, 0);
}

// ******** Earth Rotation Model ********

quatd_t EarthRotationModel::spin(double tjd) const
{
	double t = tjd - ofs::astro::J2000;
//	double theta = (PI * 2) * (t * 24.0 / 23.9344694 - 259.853 / 360.0);
	double theta = (PI * 2) * (t * 24.0 / 23.9344694 - 79.853 / 360.0);

	return yrot(-theta);
}

quatd_t EarthRotationModel::getEquatorRotation(double tjd) const
{
	double T = (tjd - astro::J2000) / 36525.0;

	if (T < -P03LP_PRECESSION_CENTURIES)
		T = -P03LP_PRECESSION_CENTURIES;
	else if (T > P03LP_PRECESSION_CENTURIES)
		T = P03LP_PRECESSION_CENTURIES;

	double opa, osa, ppa, pqa;

	computePrecissionObliquity(T, opa, osa);
	computeEclipticPrecission(T, ppa, pqa);

	double obliquity  = glm::radians(osa / 3600);
	double precession = glm::radians(ppa / 3600);

	double P = ppa * 2.0 * PI / 129600;
	double Q = pqa * 2.0 * PI / 129600;
	double piA = asin(sqrt(P*P + Q*Q));
	double PiA = atan2(P, Q);

	quatd_t RPi = zrot(PiA);
	quatd_t rpi = xrot(piA);
	quatd_t eclRotation = glm::conjugate(RPi) * rpi * RPi;

	quatd_t q = xrot(obliquity) * zrot(-precession) * glm::conjugate(eclRotation);

	return xrot(PI / 2.0) * q * xrot(-PI / 2.0);

//	return quatd_t(1, 0, 0, 0);
}
