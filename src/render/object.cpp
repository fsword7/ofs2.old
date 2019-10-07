/*
 * object.cpp - Visual object package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "render/render.h"
#include "render/object.h"
#include "render/planet.h"

vObject::vObject(Scene &scene, const Object &object)
: scene(scene), object(object)
{

}

vObject *vObject::create(const Object *object, Scene *scene)
{
	switch (object->getType())
	{
	case ObjectType::objCelestialBody:
		return new vPlanet(*scene, *object);
//	case ObjectType::objCelestialStar:
//		return new vCelestialStar(obj, scn);
	case ObjectType::objUnknown:
		return nullptr;
	}
	return nullptr;
}
