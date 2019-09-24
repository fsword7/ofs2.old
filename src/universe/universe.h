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
        
        inline const StarCatalogue *getStarCatalogue() const { return &starlib; }

        Object *find(const string& name) const;
        CelestialStar *findStar(const std::string& name) const;

        int findNearStars(const vec3d_t& obs, double mdist,
        		vector<const CelestialStar *>& stars) const;

        CelestialBody *getEarth() const { return earth; }

    private:
        StarCatalogue starlib;

        CelestialBody *earth;
    };
}
