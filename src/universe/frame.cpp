/*
 * frame.cpp - reference frame package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"

using namespace ofs::universe;

// ******** System Tree ********

SystemTree::SystemTree(Object *object)
{
	switch (object->getType()) {
	case ObjectType::objCelestialStar:
		starParent = dynamic_cast<CelestialStar *>(object);
		break;
	case ObjectType::objCelestialBody:
		bodyParent = dynamic_cast<CelestialBody *>(object);
		break;
	}
}

void SystemTree::addObject(Object *object)
{
	objects.push_back(object);
}

Object *SystemTree::getObject(int idx) const
{
	if (idx < 0)
		return nullptr;
	if (idx <= objects.size())
		return objects[idx];
	return nullptr;
}

// ******** Reference Frame ********

ReferenceFrame::ReferenceFrame(const Object *object)
: center(object)
{
}

int ReferenceFrame::lock() const
{
	return ++refCount;
}

int ReferenceFrame::release() const
{
	int count = --refCount;
	if (refCount <= 0)
		delete this;
	return count;
}

//vec3d_t ReferenceFrame::fromUniversal(const vec3d_t& upos, double tjd)
//{
////	vec3d_t pos;
//
//	if (center == nullptr)
//		return upos;
//	return getRotation(tjd).conjugate().toRotationMatrix() * (center->getPosition(tjd) - upos);
//
////	std::cout << std::fixed << std::setprecision(10);
////	std::cout << "Time: (from) " << tjd << " (" << pos.x() << "," << pos.y() << "," << pos.z() << ")" << std::endl;
////	return pos;
//}

//vec3d_t ReferenceFrame::toUniversal(const vec3d_t& lpos, double tjd)
//{
////	vec3d_t pos;
//
//	if (center == nullptr)
//		return lpos;
//	return center->getPosition(tjd) + (getRotation(tjd).toRotationMatrix() * lpos);
//
////	pos = center->getPosition(tjd) + lpos;
////	std::cout << std::fixed << std::setprecision(10);
////	std::cout << "Time: (to) " << tjd << " (" << pos.x() << "," << pos.y() << "," << pos.z() << ")" << std::endl;
////	std::cout << "Time:      " << tjd << " (" << lpos.x() << "," << lpos.y() << "," << lpos.z() << ")" << std::endl;
////	return pos;
//}

//quatd_t ReferenceFrame::fromUniversal(const quatd_t& urot, double tjd)
//{
//	if (center == nullptr)
//		return urot;
//	return urot * getRotation(tjd).conjugate();
//}

//quatd_t ReferenceFrame::toUniversal(const quatd_t& lrot, double tjd)
//{
//	if (center == nullptr)
//		return lrot;
//	return lrot * getRotation(tjd);
//}

// *******************************************

//EclipticFrame::EclipticFrame(const Object *obj)
//: ReferenceFrame(obj)
//{
//}
//
//EclipticFrame::~EclipticFrame()
//{
//}

// *******************************************

//EquatorFrame::EquatorFrame(const Object *obj, const Object *tgt)
//: ReferenceFrame(obj)
//{
//}
//
//EquatorFrame::~EquatorFrame()
//{
//}

// *******************************************

//BodyMeanFrame::BodyMeanFrame(const Object *obj, const Object *tgt)
//: ReferenceFrame(obj)
//{
//}
//
//BodyMeanFrame::~BodyMeanFrame()
//{
//}
