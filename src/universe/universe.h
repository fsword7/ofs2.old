/*
 * universe.h - Universe package
 *
 *  Created on: Aug 25, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/starlib.h"
#include "universe/body.h"

using namespace ofs::universe;

namespace ofs::universe {
    class Universe
    {
    public:
        Universe() = default;
        ~Universe() = default;

        void init();
        
    private:
        StarCatalogue starlib;

        CelestialBody *earth;
    };
}