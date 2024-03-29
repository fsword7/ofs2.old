/*
 * body.cpp - Celestial body package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/orbit.h"
#include "universe/astro.h"
#include "universe/body.h"

using namespace ofs::astro;
using namespace ofs::universe;

// ******** Hierarchical Planetary System ********

//PlanetarySystem::PlanetarySystem()
//{
//	bodies.clear();
//}

PlanetarySystem::PlanetarySystem(CelestialBody *body)
: body(body), tree(body)
{
	bodies.clear();
}

PlanetarySystem::PlanetarySystem(CelestialStar *star)
: star(star), tree(star)
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

CelestialBody *PlanetarySystem::find(const string &name) const
{
	for (auto obj : bodies) {
//		if (boost::iequals(name, obj->getName()))
		if (name == obj->getName())
			return obj;
	}
	return nullptr;
}

// ********** Celestial Body ************

//CelestialBody::CelestialBody()
//: Object(ObjectType::objCelestialBody),
//  frame(this)
//{
//}

CelestialBody::CelestialBody(PlanetarySystem *system, const string &name,
	CelestialType type)
: Object(ObjectType::objCelestialBody, name),
  bodyType(type)
{
	inSystem = system;
	system->addBody(this);
}


CelestialBody::CelestialBody(const string &name, CelestialType type,
	CelestialBody *body)
: Object(ObjectType::objCelestialBody, name),
  bodyType(type)
{
	if (body != nullptr) {
		inSystem = body->createPlanetarySystem();
		inSystem->addBody(this);
	}
}

CelestialBody::~CelestialBody()
{
	if (inSystem != nullptr)
		inSystem->removeBody(this);
}

PlanetarySystem *CelestialBody::createPlanetarySystem()
{
	if (ownSystem != nullptr)
		return ownSystem;
	ownSystem = new PlanetarySystem(this);
	ownSystem->setStar(inSystem->getStar());
	return ownSystem;
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

//vec3d_t CelestialBody::getGlobalPosition(double tjd) const
//{
//	vec3d_t upos = {0, 0, 0};
//	Frame *frame = orbitFrame;
//	vec3d_t lpos = orbit->getPosition(tjd);
//
//	while (frame->getCenter()->getType() == objCelestialBody) {
//		upos  += glm::conjugate(frame->getOrientation(tjd)) * lpos;
//		cout << fmt::sprintf("Object %s: %lf, %lf, %lf\n",
//			frame->getCenter()->getName(),
//			upos.x, upos.y, upos.z);
//		lpos = frame->getCenter()->getPosition(tjd);
//		frame  = frame->getCenter()->getOrbitFrame();
//	}
//
//	return frame->getCenter()->getPosition(tjd) + upos;
//}

quatd_t CelestialBody::getRotation(double tjd) const
{
	if (rot == nullptr)
		return objRotation;
	return rot->getRotation(tjd);
//	return rot->spin(tjd) * rot->getEquatorRotation(tjd);
}

quatd_t CelestialBody::getOrientation(double tjd) const
{
	return rot->getRotation(tjd) * objectFrame->getOrientation(tjd);
}

double CelestialBody::getLuminosity(double lum, double dist) const
{
	double power           = lum * SOLAR_POWER;
	double irradiance      = power / sphereArea(dist * 1000);
	double incidentEnergy  = irradiance * circleArea(objRadius * 1000);
	double reflectedEnergy = incidentEnergy * geomAlbedo;

	return reflectedEnergy / SOLAR_POWER;
}

double CelestialBody::getApparentMagnitude(vec3d_t sun, double irradiance, vec3d_t view) const
{
	double vdist  = glm::length(view);
	double sdist  = glm::length(sun);
	double illum  = (1.0 + glm::dot(view / vdist, sun / sdist)) / 2.0;
	double absMag = getLuminosity(irradiance, sdist) * illum;

	return astro::convertLumToAppMag(absMag, astro::convertKilometerToParsec(vdist));
}

vec3d_t CelestialBody::getHeliocentric(double jd) const
{
	vec3d_t opos = getPosition(jd);
	vec3d_t hpos = glm::normalize(opos);

	double lat = acos(hpos.y) - (PI / 2);
	double lon = atan2(hpos.z, -hpos.x);

	return vec3d_t(lon, lat, glm::length(opos));
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
	vec3d_t lpos = pos * glm::conjugate(getBodyFixed(tdb));

	return getPlanetocentric(lpos);
}
