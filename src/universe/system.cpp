/*
 * system.cpp - solar system package
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/frame.h"
#include "universe/system.h"

using namespace ofs::universe;

// ******** System Tree ********

SystemTree::SystemTree(Object *object)
{
	switch (object->getType()) {
	case ObjectType::objCelestialStar:
		starParent   = dynamic_cast<CelestialStar *>(object);
		defaultFrame = new J2000EclipticFrame(object);
		break;
	case ObjectType::objCelestialBody:
		bodyParent   = dynamic_cast<CelestialBody *>(object);
		defaultFrame = new BodyMeanEquatorFrame(object, object);
		break;
	}
}

SystemTree::~SystemTree()
{
	if (defaultFrame != nullptr)
		defaultFrame->release();
}

void SystemTree::addObject(Object *object)
{
	object->setOrbitFrame(defaultFrame);
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

// ******** Solar System ********

System::System(CelestialStar *star)
: systemTree(star), objects(star)
{
	stars.clear();
	stars.push_back(star);

	star->setSystem(this);
}

void System::addObject(Object *object)
{
	systemTree.addObject(object);

	switch(object->getType())
	{
	case objCelestialBody:
		CelestialBody *body = dynamic_cast<CelestialBody *>(object);
		objects.addBody(body);
		break;
	}
}


