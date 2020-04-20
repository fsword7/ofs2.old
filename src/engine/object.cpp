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
: objType(type), objNames(1)
{
    objNames.clear();
}

Object::Object(ObjectType type, const string &name)
: objType(type), objNames(1)
{
	objNames.clear();
    objNames[0] = name;
}

Object::~Object()
{
    objNames.clear();
}

double Object::computeCullingRadius()
{
	return objRadius;
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

quatd_t Object::getRotation(double tjd) const
{
	if (rot != nullptr)
		return rot->getRotation(tjd);
	return objRotation;
}

//SystemTree *Object::createSystemTree()
//{
//	if (sysTree == nullptr)
//		sysTree = new SystemTree(this);
//	return sysTree;
//}
