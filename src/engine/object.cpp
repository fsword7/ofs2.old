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

vec3d_t Object::getGlobalPosition(double tjd) const
{
	vec3d_t upos = {0, 0, 0};
	Frame *frame = orbitFrame;
	vec3d_t lpos = orbit->getPosition(tjd);

//	cout << fmt::sprintf("Object: %s => (%lf, %lf, %lf)\n",
//		getName(), lpos.x, lpos.y, lpos.z) << flush;

//	for (Frame *frame = orbitFrame; !frame->isRoot(); frame = frame->getParentFrame()) {
//		lpos = orbit->getPosition(tjd);
//		upos += glm::conjugate(frame->getOrientation(tjd)) * lpos;
//	}

	while (frame->getCenter()->getType() == objCelestialBody) {
		upos  += glm::conjugate(frame->getOrientation(tjd)) * lpos;
		lpos = frame->getCenter()->getPosition(tjd);
//		cout << fmt::sprintf("Object: %s => (%lf, %lf, %lf) now (%lf, %lf, %lf)\n",
//			frame->getCenter()->getName(),
//			lpos.x, lpos.y, lpos.z, upos.x, upos.y, upos.z);
		frame  = frame->getCenter()->getOrbitFrame();
	}
	upos  += glm::conjugate(frame->getOrientation(tjd)) * lpos;
//	cout << fmt::sprintf("Final (%lf, %lf, %lf)\n", upos.x, upos.y, upos.z);

	return upos;
//	return frame->getCenter()->getPosition(tjd) + upos;
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

quatd_t Object::getOrientation(double tjd) const
{
	return rot->getRotation(tjd) * objectFrame->getOrientation(tjd);
}

//quatd_t Object::getGeometeryOrientation() const
//{
//	return objOrientation;
//}

//SystemTree *Object::createSystemTree()
//{
//	if (sysTree == nullptr)
//		sysTree = new SystemTree(this);
//	return sysTree;
//}
