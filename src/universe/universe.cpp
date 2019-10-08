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

using namespace ofs::astro;
using namespace ofs::universe;

void Universe::init()
{
	// Loading star catalogue
	std::string pname = "data/stars/xhip";
	starlib.loadXHIPData(pname);

	std::string cname = "data/constellations/western/constellationship.fab";
//	std::string cname = "data/constellations/western_rey/constellationship.fab";
	asterism.load(cname);

	// To removed later...

	CelestialStar *sun = findStar("Sol");
	System *solSystem = new System(sun);

	earth = new CelestialBody("earth");

	// Earth orbit parameters
	double T     = 365.256;     // Period (T) [days]
	double a     = 1.00000011;  // Semimajor Axis (a) [AU]
	double e     = 0.01671022;  // Eccentricity (e)
	double i     = 0.00005;     // Inclination (i) [deg]
	double Omega = -11.26064;   // Longitude of ascending node (Omega) [deg]
	double n     = 348.7;       // Ascending node (n) [deg]
	double W     = 102.94719;   // Longitude of perihelion (W) [deg]
	double L     = 100.46435;   // Mean longitude at Epoch (L0) [deg]

//	double w     = 288.1;       // Argument of perigee (w)

	double w     = W - n;        // Argument of perigee (w)
	double M0    = L - (w + n);  // Mean Anomaly at Epoch (M0)

	Orbit *orbit = new EllipticalOrbit(a, e, glm::radians(i),
			glm::radians(Omega), glm::radians(w), glm::radians(M0), T, SOLAR_MASS);

//	earth->setOrbit(orbit);
    earth->setPosition(vec3d_t(0.0, 0.0, -149.6e6));
    earth->setRadius(6371.0);

    solSystem->addObject(earth);
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

//Object *Universe::findChildObject(Object *obj, const string& name) const
//{
//	SolarSystem   *sys;
//	PlanetSystem  *objsys;
//	CelestialBody *body;
//
//	switch(obj->getType()) {
//	case Object::objCelestialStar:
//		sys = (dynamic_cast<CelestialStar *>(obj))->getSolarSystem();
//		if (sys != nullptr) {
//			objsys = sys->getBodies();
//			if (objsys != nullptr) {
//				body = objsys->find(name, false);
//				if (body != nullptr)
//					return body;
//			}
//		}
//		break;
//	case Object::objCelestialBody:
//		break;
//	}
//	return nullptr;
//}

//Object *Universe::findPath(const string& path) const
//{
//	string::size_type pos = path.find('/', 0);
//
//	if (pos == string::npos)
//		return find(path);
//
//	string base(path, 0, pos);
//	Object *obj = find(base);
//
//	while(obj != nullptr && pos != string::npos) {
//		string::size_type nextPos = path.find('/', pos+1);
//		string::size_type len;
//
//		len = ((nextPos == string::npos) ? path.size() : nextPos) - pos - 1;
//		string name = string(path, pos+1, len);
//
//		obj = findChildObject(obj, name);
//		pos = nextPos;
//	}
//
//	return obj;
//}

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
