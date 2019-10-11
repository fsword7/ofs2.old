/*
 * body.h - Celestial body package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "engine/object.h"

namespace ofs::universe {
	class System;
	class CelestialBody;
	class CelestialStar;

	class PlanetarySystem
	{
	public:
		PlanetarySystem(CelestialBody *body = nullptr);
		PlanetarySystem(CelestialStar *star);
		~PlanetarySystem() = default;

		inline CelestialBody *getPrimaryBody() { return body; }
		inline CelestialStar *getStar() const { return star; }
		inline int getSystemSize() { return bodies.size(); }

		void addBody(CelestialBody *body);
		void removeBody(CelestialBody *body);

	private:
		// Planetary system parameters
		System *solarSystem = nullptr;
		CelestialBody *body = nullptr;
		CelestialStar *star = nullptr;
		vector<CelestialBody *> bodies;
	};

    class CelestialBody : public Object
    {
    public:
        CelestialBody();
        CelestialBody(const string &name, PlanetarySystem *system = nullptr);
        ~CelestialBody() = default;

        inline void setInSystem(PlanetarySystem *system) { inSystem = system; }

        inline PlanetarySystem *getOwnSystem() const { return ownSystem; }
        inline PlanetarySystem *getInSystem() const { return inSystem; }

    private:
//        PlanetarySystem ownSystem;
        PlanetarySystem *ownSystem = nullptr;
        PlanetarySystem *inSystem = nullptr;
    };
}
