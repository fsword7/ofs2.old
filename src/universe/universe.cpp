/*
 * universe.h - Universe package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
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
    earth1 = new CelestialBody();
    earth1->setName("earth1");

    earth1->setPosition(vec3f_t(0.0f, 0.0f, -20000.0f));
    earth1->setRadius(6371.0f);

    earth2 = new CelestialBody();
    earth2->setName("earth2");
  
    earth2->setPosition(vec3f_t(0.0f, 0.0f, -40000.0f));
    earth2->setRadius(6371.0f);

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
