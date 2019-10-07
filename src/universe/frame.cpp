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
