/*
 * system.cpp - solar system package
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/universe.h"
#include "universe/frame.h"
#include "universe/system.h"

using namespace ofs::universe;

// ******** Reference Frame Tree ********

SystemTree::SystemTree(Object *object)
{
	switch (object->getType()) {
	case ObjectType::objCelestialStar:
		starParent   = dynamic_cast<CelestialStar *>(object);
		defaultFrame = new J2000EclipticFrame(object, nullptr);
		break;
	case ObjectType::objCelestialBody:
		bodyParent   = dynamic_cast<CelestialBody *>(object);
		defaultFrame = new BodyMeanEquatorFrame(object, object, nullptr);
		break;
	}
}

SystemTree::SystemTree(CelestialStar *star)
{
	starParent = star;
	defaultFrame = new J2000EclipticFrame(star, nullptr);
}

SystemTree::SystemTree(CelestialBody *body)
{
	bodyParent = body;
	defaultFrame = new BodyMeanEquatorFrame(body, body, nullptr);
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

CelestialBody *System::createBody(const string &name, PlanetarySystem *system,
	CelestialType type, const string &orbitFrameName, const string &bodyFrameName)
{
	CelestialBody *body = new CelestialBody(system, name, type);

	Object *parentObject = system->getPrimaryBody();
	if (parentObject == nullptr) {
		parentObject = system->getStar();
	}

	cout << fmt::sprintf("Body %s -> %s\n", name, parentObject->getName()) << flush;

	FrameTree *parentFrame = system->getSystemTree();
	Frame *defaultOrbitFrame = parentFrame->getDefaultReferenceFrame();
	Frame *defaultBodyFrame = parentFrame->getDefaultReferenceFrame();

	parentFrame->addObject(body);

	Frame *orbitFrame, *bodyFrame;

	if (!orbitFrameName.empty())
		orbitFrame = Frame::create(orbitFrameName, parentObject, parentObject);
	else
		orbitFrame = defaultOrbitFrame;

	if (!bodyFrameName.empty())
		bodyFrame = Frame::create(bodyFrameName, body);
	else
		bodyFrame = defaultBodyFrame;

	cout << fmt::sprintf("Orbit Frame: %s (Center: %s)\n",
		orbitFrame->getName(), orbitFrame->getCenter()->getName());
	cout << fmt::sprintf("Body Frame: %s (Center: %s)\n",
		bodyFrame->getName(), bodyFrame->getCenter()->getName());

	body->setOrbitFrame(orbitFrame);
	body->setObjectFrame(bodyFrame);

	return body;
}
