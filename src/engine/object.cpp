/*
 * object.cpp - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/orbit.h"
#include "engine/object.h"

Object::Object(ObjectType type)
: objType(type), objNames(1),
  objRadius(0), objSemiAxes(0, 0, 0),
  objMass(0), objAlbedo(1.0),
  objPosition(0, 0, 0)
{
    objNames.clear();
}

Object::Object(ObjectType type, const string &name)
: objType(type), objNames(1),
  objRadius(0), objSemiAxes(0, 0, 0),
  objMass(0), objAlbedo(1.0),
  objPosition(0, 0, 0)
{
    objNames[0] = name;
}

Object::~Object()
{
    objNames.clear();
}

vec3d_t Object::getPosition(double tjd) const
{
	if (orbit != nullptr)
		return orbit->getPosition(tjd);
	return objPosition;
}

vec3d_t Object::getVelocity(double tjd) const
{
	if (orbit != nullptr)
		return orbit->getVelocity(tjd);
	return vec3d_t(0, 0, 0);
}

quatd_t Object::getRotation(double) const
{
	return objRotation;
}
