/*
 * object.cpp - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
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

vec3f_t Object::getPosition(float) const
{
	return objPosition;
}

quatf_t Object::getRotation(float) const
{
	return objRotation;
}
