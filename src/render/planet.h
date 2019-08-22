/*
 * planet.h - Visual planet package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/object.h"
#include "render/terrain.h"

class vPlanet : public vObject
{
    public:
        vPlanet(const Scene &scene);
        ~vPlanet() = default;

    protected:
        TerrainManager tmgr;
};