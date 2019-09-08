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
    // earth->setPosition(vec3f_t(0.0f, 0.0f, -20000.0f));
    // earth->setRadius(6371.0f);
    earth->setPosition(vec3f_t(0.0f, 0.0f, -3.0f));
    earth->setRadius(1.0f);

}