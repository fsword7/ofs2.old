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

PlanetarySystem::PlanetarySystem(CelestialBody *body)
: body(body)
{
}

PlanetarySystem::PlanetarySystem(CelestialStar *star)
: star(star)
{
}

void PlanetarySystem::addBody(CelestialBody *body)
{
	bodies.push_back(body);
}

void PlanetarySystem::removeBody(CelestialBody *body)
{
}

// ********** Celestial Body ************

CelestialBody::CelestialBody()
: Object(ObjectType::objCelestialBody)// , PlanetarySystem(this)
{

}

CelestialBody::CelestialBody(const string &name, PlanetarySystem *system)
: Object(ObjectType::objCelestialBody, name), // PlanetarySystem(this),
  system(system)
{
	if (system != nullptr)
		system->addBody(this);
}
