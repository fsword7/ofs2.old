/*
 * body.cpp - Celestial body package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/orbit.h"
#include "universe/body.h"

using namespace ofs::universe;

// ******** Planetary System ********

//PlanetarySystem::PlanetarySystem()
//{
//	bodies.clear();
//}

PlanetarySystem::PlanetarySystem(CelestialBody *body)
: body(body)
{
	bodies.clear();
}

PlanetarySystem::PlanetarySystem(CelestialStar *star)
: star(star)
{
	bodies.clear();
}

void PlanetarySystem::addBody(CelestialBody *body)
{
	bodies.push_back(body);
	body->setInSystem(this);
}

void PlanetarySystem::removeBody(CelestialBody *body)
{
}

// ********** Celestial Body ************

CelestialBody::CelestialBody()
: Object(ObjectType::objCelestialBody) //, ownSystem(this)
{
	ownSystem = new PlanetarySystem(this);
}

CelestialBody::CelestialBody(const string &name, PlanetarySystem *system)
: Object(ObjectType::objCelestialBody, name), inSystem(system) //, ownSystem(this)
{
	ownSystem = new PlanetarySystem(this);
	if (inSystem != nullptr)
		inSystem->addBody(this);
}
