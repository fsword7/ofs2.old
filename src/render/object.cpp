/*
 * object.cpp - Visual object package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/render.h"
#include "render/object.h"

vObject::vObject(const Scene &scene, const Object &object)
: scene(scene), object(object)
{

}