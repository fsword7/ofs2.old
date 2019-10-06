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
		PlanetarySystem() = default;
		~PlanetarySystem() = default;

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
        ~CelestialBody() = default;

    private:
    };
}
