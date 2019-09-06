/*
 * planet.cpp - Visual planet package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/planet.h"

vPlanet::vPlanet(Scene &scene, Object &object)
: vObject(scene, object)
{
    tmgr = new TerrainManager(scene, object);
}

void vPlanet::render(renderParameter &prm)
{
    tmgr->render(prm);
}