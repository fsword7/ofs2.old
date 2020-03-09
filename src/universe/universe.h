/*
 * universe.h - Universe package
 *
 *  Created on: Aug 25, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/asterisms.h"
#include "universe/starlib.h"
#include "universe/body.h"

using namespace ofs::universe;

class Engine;

namespace ofs::universe {
    class Universe
    {
    public:
        Universe() = default;
        ~Universe() = default;

        void init();
        
        inline const StarCatalogue *getStarCatalogue() const { return &starlib; }
        inline const Constellations &getAsterism() const { return asterism; }

        inline const Engine *getEngine() const { return engine; }

        inline void setEngine(Engine *engine) { this->engine = engine; }

        Object *find(const string& name) const;
        CelestialStar *findStar(const std::string& name) const;
        Object *findObject(const Object *obj, const string &name) const;
        Object *findPath(const string &path) const;

        int findNearStars(const vec3d_t& obs, double mdist,
        		vector<const CelestialStar *>& stars) const;

        CelestialBody *getEarth() { return earth; }

    private:
        StarCatalogue starlib;
        Constellations asterism;

        Engine *engine = nullptr;

        CelestialBody *earth = nullptr;
    };
}
