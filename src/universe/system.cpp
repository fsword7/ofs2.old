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
}


