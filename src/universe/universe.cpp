/*
 * universe.h - Universe package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/universe.h"

void Universe::init()
{
    
    earth = new CelestialBody();
    earth->setName("earth");
    earth->setRadius(2.0f);
}