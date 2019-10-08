/*
 * rotation.cpp - OFS Rotation Model package
 *
 *  Created on: Oct 8, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/rotation.h"

using namespace ofs::ephem;

quatd_t RotationModel::getEquatorRotation(double tjd) const
{
	return quatd_t(1, 0, 0, 0);
}

//quatd_t RotationModel::getRotation(double tjd) const;
//{
//	return spin(tjd) * getEquatorRotation(tjd);
//}


