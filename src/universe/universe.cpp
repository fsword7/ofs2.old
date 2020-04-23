/*
 * universe.h - Universe package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "universe/system.h"
#include "universe/universe.h"
#include "ephem/vsop87.h"

using namespace ofs::astro;
using namespace ofs::universe;
using namespace ofs::ephem;

void Universe::init()
{
	// Loading star catalogue
	std::string pname = "data/stars/xhip";
	starlib.loadXHIPData(pname);

	std::string cname = "data/constellations/western/constellationship.fab";
//	std::string cname = "data/constellations/western_rey/constellationship.fab";
	asterism.load(cname);

	// To removed later...

	CelestialBody *mercury, *venus, *earth, *mars;
	CelestialBody *jupiter, *saturn, *uranus, *neptune;
	CelestialBody *moon;

	CelestialStar *sun = findStar("Sol");
	System *solSystem = new System(sun);

	mercury = new CelestialBody("Mercury", CelestialBody::ctPlanet);
	venus   = new CelestialBody("Venus", CelestialBody::ctPlanet);
	earth   = new CelestialBody("Earth", CelestialBody::ctPlanet);
	moon    = new CelestialBody("Moon", CelestialBody::ctMoon, earth);
	mars    = new CelestialBody("Mars", CelestialBody::ctPlanet);
	jupiter = new CelestialBody("Jupiter", CelestialBody::ctPlanet);
	saturn  = new CelestialBody("Saturn", CelestialBody::ctPlanet);
	uranus  = new CelestialBody("Uranus", CelestialBody::ctPlanet);
	neptune = new CelestialBody("Neptune", CelestialBody::ctPlanet);

//	Orbit *orbit = new EllipticalOrbit(a, e, glm::radians(i),
//			glm::radians(Omega), glm::radians(w), glm::radians(M0), T, SOLAR_MASS);

    mercury->setColor(Color(1.0, 0.794, 0.580));
    mercury->setOrbit(VSOP87Orbit::create("vsop87-mercury"));
    mercury->setRadius(2439.7);
    mercury->setGeometricAlbedo(0.141380);
    mercury->setBondAlbedo(0.088);
    mercury->enableSurface();

    venus->setOrbit(VSOP87Orbit::create("vsop87-venus"));
    venus->setRadius(6051.8);
    venus->setGeometricAlbedo(0.672604);
    venus->setBondAlbedo(0.760);
    venus->enableSurface();

    earth->setColor(Color(0.856, 0.910, 1.0));
    earth->setOrbit(VSOP87Orbit::create("vsop87-earth"));
	earth->setRotation(new EarthRotationModel());
    earth->setRadius(6378.140);
    earth->setGeometricAlbedo(0.449576);
    earth->setBondAlbedo(0.306);
    earth->enableSurface();

//    moon->setOrbit(VSOP87Orbit::create("elp82a-lunar"));
    moon->setRadius(1738.14);
    moon->setGeometricAlbedo(0.136);
    moon->setBondAlbedo(0.11);
    moon->enableSurface();

    mars->setColor(Color(1.0, 0.75, 0.7));
    mars->setOrbit(VSOP87Orbit::create("vsop87-mars"));
    mars->setRadius(3396.2);
    mars->setGeometricAlbedo(0.174821);
    mars->setBondAlbedo(0.290);
    mars->enableSurface();

    jupiter->setColor(Color(1.0, 0.908, 0.720));
    jupiter->setOrbit(VSOP87Orbit::create("vsop87-jupiter"));
    jupiter->setRadius(71492.0);
    jupiter->setGeometricAlbedo(0.510901);
    jupiter->setBondAlbedo(0.503);
//    jupiter->enableSurface();

    saturn->setColor(Color(1.0, 0.735, 0.486));
    saturn->setOrbit(VSOP87Orbit::create("vsop87-saturn"));
    saturn->setRadius(60268.0);
    saturn->setGeometricAlbedo(0.499740);
    saturn->setBondAlbedo(0.342);
    saturn->enableSurface();

    uranus->setColor(Color(0.606, 0.948, 1.0));
    uranus->setOrbit(VSOP87Orbit::create("vsop87-uranus"));
    uranus->setRadius(25559.0);
    uranus->setGeometricAlbedo(0.437118);
    uranus->setBondAlbedo(0.300);
    uranus->enableSurface();

    neptune->setColor(Color(0.523, 0.848, 1.0));
    neptune->setOrbit(VSOP87Orbit::create("vsop87-neptune"));
    neptune->setRadius(24766.0);
    neptune->setGeometricAlbedo(0.409338);
    neptune->setBondAlbedo(0.290);
    neptune->enableSurface();

    solSystem->addObject(mercury);
    solSystem->addObject(venus);
    solSystem->addObject(earth);
    solSystem->addObject(mars);
    solSystem->addObject(jupiter);
    solSystem->addObject(saturn);
    solSystem->addObject(uranus);
    solSystem->addObject(neptune);
}

//SolarSystem *Universe::createSolarSystem(CelestialStar *star)
//{
//	SolarSystem *solsys;
//
//	solsys = getSolarSystem(star);
//	if (solsys != nullptr)
//		return solsys;
//
//	solsys = new SolarSystem(star);
//
//	return solsys;
//}

//SolarSystem *Universe::getSolarSystem(const CelestialStar *star) const
//{
//	if (star == nullptr)
//		return nullptr;
//
//	return star->getSolarSystem();
//}

CelestialStar *Universe::findStar(const std::string& name) const
{
	return starlib.find(name);
}

Object *Universe::find(const string& name) const
{
	Object *obj = starlib.find(name);
	if (obj != nullptr)
		return obj;
	return nullptr;
}

Object *Universe::findObject(const Object *obj, const string &name) const
{
	System *sys;
	const CelestialStar *sun;
	const CelestialBody *body;
	const PlanetarySystem *objects;

	switch (obj->getType()) {
	case objCelestialStar:
		sun = dynamic_cast<const CelestialStar *>(obj);
		if ((sys = sun->getSystem()) == nullptr)
			break;
		objects = sys->getCelestialBodies();
		return objects->find(name);

	case objCelestialBody:
		body = dynamic_cast<const CelestialBody *>(obj);
		objects = body->getOwnSystem();
		if (objects != nullptr)
			return objects->find(name);
		break;

	}

	// Otherwise, find nothing
	return nullptr;
}

Object *Universe::findPath(const string &path) const
{
	string::size_type pos = path.find('/', 0);
	if (pos == string::npos)
		return find(path);

	string base(path, 0, pos);
	Object *obj = find(base);

	while(obj != nullptr && pos != string::npos) {
		string::size_type npos = path.find('/', pos+1);
		string::size_type len;

		len = ((npos == string::npos) ? path.size() : npos) - pos - 1;
		string name = string(path, pos+1, len);

		obj = findObject(obj, name);
		pos = npos;
	}

	return obj;
}

//Object *Universe::pickStar() const
//{
//	return nullptr;
//}

//Object *Universe::pickPlanet() const
//{
//	return nullptr;
//}

Object *Universe::pickObject()
{
//	vec3d_t origin = player->getPosition();

	closeStars.clear();
//	findNearStars(origin, 1.0, closeStars);
//	cout << "Closest star list: (" << nearStars.size() << " stars)" << endl;
//	for(const CelestialStar *star : nearStars) {
//		if (star->getHIPNumber() == 0)
//			cout << "HIP " << star->getHIPNumber() << " " << star->getName()
//			<< " Distance: " << glm::length(star->getPosition(0)) << endl;
//	}

//	for (int idx = 0; idx < nearStars.size(); idx++) {
//		const CelestialStar *sun = nearStars[idx];
//
//		if (!sun->hasSystem())
//			continue;
//		obj = pickPlanet(sun->getSystem());
//	}

	return nullptr;
}

//class NearStarHandler : public ofsHandler
//{
//public:
//	NearStarHandler(double mdist, std::vector<const CelestialStar *> &stars)
//		: maxDistance(mdist), nStars(stars) {};
//	~NearStarHandler() {};
//
//	void process(const CelestialStar& star, double dist, double) const
//	{
//		if (dist < maxDistance)
//			nStars.push_back(&star);
//	}
//
//private:
//	double maxDistance;
//	std::vector<const CelestialStar *> &nStars;
//
//};

int Universe::findNearStars(const vec3d_t& obs, double mdist,
		vector<const CelestialStar *>& stars) const
{
//	NearStarHandler handle(mdist, stars);
	int count;

	starlib.findNearStars(obs / KM_PER_PC, mdist, stars);

//	cout << "Near stars: " << count << " stars" << std::endl;
//	if (count > 0)
//		cout << "Star name: " << stars[0]->getName() << std::endl;

	return stars.size();
}
