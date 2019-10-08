/*
 * object.cpp - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/orbit.h"
#include "engine/object.h"

using namespace ofs::ephem;

Object::Object(ObjectType type)
: objType(type), objNames(1),
  objRadius(0), objSemiAxes(0, 0, 0),
  objMass(0), objAlbedo(1.0),
  objPosition(0, 0, 0),
  objVelocity(0, 0, 0)
{
    objNames.clear();
}

Object::Object(ObjectType type, const string &name)
: objType(type), objNames(1),
  objRadius(0), objSemiAxes(0, 0, 0),
  objMass(0), objAlbedo(1.0),
  objPosition(0, 0, 0),
  objVelocity(0, 0, 0)
{
	objNames.clear();
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
	return objVelocity;
}

quatd_t Object::getRotation(double) const
{
	return objRotation;
}

//SystemTree *Object::createSystemTree()
//{
//	if (sysTree == nullptr)
//		sysTree = new SystemTree(this);
//	return sysTree;
//}
