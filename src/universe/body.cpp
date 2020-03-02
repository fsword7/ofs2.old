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
: Object(ObjectType::objCelestialBody), frame(this), ownSystem(this)
{
//	ownSystem = new PlanetarySystem(this);
}

CelestialBody::CelestialBody(const string &name, PlanetarySystem *system)
: Object(ObjectType::objCelestialBody, name), frame(this), inSystem(system), ownSystem(this)
{
//	ownSystem = new PlanetarySystem(this);
	if (inSystem != nullptr)
		inSystem->addBody(this);
}

quatd_t CelestialBody::getBodyFixed(double tjd) const
{
	if (rot == nullptr)
		return objRotation;
	return rot->getRotation(tjd);
}

quatd_t CelestialBody::getEquatorial(double tjd) const
{
	if (rot == nullptr)
		return objRotation;
	return rot->getRotation(tjd);
}

quatd_t CelestialBody::getRotation(double tjd) const
{
	if (rot == nullptr)
		return objRotation;
	return rot->getRotation(tjd);
//	return rot->spin(tjd) * rot->getEquatorRotation(tjd);
}

vec3d_t CelestialBody::getPlanetocentric(const vec3d_t &pos) const
{
	vec3d_t w = glm::normalize(pos);

	double lat = acos(w.y) - (PI / 2);
	double lon = atan2(w.z, -w.x);

	return vec3d_t(lon, lat, glm::length(pos) - getRadius());
}

vec3d_t CelestialBody::getPlanetocentricFromEcliptic(const vec3d_t &pos, double tdb) const
{
	vec3d_t lpos = pos * getBodyFixed(tdb);

	return getPlanetocentric(lpos);
}
