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
		PlanetarySystem() = default;
		~PlanetarySystem() = default;

		inline CelestialBody *getPrimaryBody() { return body; }
		inline CelestialStar *getStar() { return star; }
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

    class CelestialBody : public Object, public PlanetarySystem
    {
    public:
        CelestialBody();
        CelestialBody(const string &name, PlanetarySystem *system = nullptr);
        ~CelestialBody() = default;

        inline PlanetarySystem *getSystem() { return system; }

    private:
        PlanetarySystem *system = nullptr;
    };
}
