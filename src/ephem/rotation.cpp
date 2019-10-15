/*
 * rotation.cpp - OFS Rotation Model package
 *
 *  Created on: Oct 8, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/rotation.h"

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

// ******** Earth Rotation Model ********

quatd_t EarthRotationModel::spin(double tjd) const
{
	double t = tjd - ofs::astro::J2000;
//	double theta = (PI * 2) * (t * 24.0 / 23.9344694 - 259.853 / 360.0);
	double theta = (PI * 2) * (t * 24.0 / 23.9344694 - 79.853 / 360.0);

	return yrot(-theta);
//	return quatd_t(vec3d_t(0, -theta, 0));
}

quatd_t EarthRotationModel::getEquatorRotation(double tjd) const
{
	return quatd_t(1, 0, 0, 0);
}
