/*
 * body.h - Celestial body package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "engine/object.h"
#include "ephem/rotation.h"

//using ofs::ephem;

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

		CelestialBody *find(const string &name) const;

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
		enum CelestialType
		{
			ctUnknown = 0,
			ctPlanet,
			ctDwarfPlamet,
			ctMinorPlanet,
			ctAsteroid,
			ctComet,
			ctMoon,
			ctSubmoon
		};

        CelestialBody();
        CelestialBody(const string &name, CelestialType type,
        	CelestialBody *body = nullptr);
        ~CelestialBody() = default;

        PlanetarySystem *createPlanetarySystem();

        inline void setInSystem(PlanetarySystem *system) { inSystem = system; }

        inline CelestialType getCelestialType() const { return bodyType; }
        inline PlanetarySystem *getOwnSystem() const { return ownSystem; }
        inline PlanetarySystem *getInSystem() const { return inSystem; }

        inline FrameTree *getReferenceFrameTree() { return &frame; }

//        inline RotationModel *getRotationModel(double tjd) const { return rot; };

        quatd_t getEquatorial(double tjd) const;
        quatd_t getBodyFixed(double tjd) const;

        quatd_t getRotation(double tjd = 0) const override;

        vec3d_t getPlanetocentric(const vec3d_t &pos) const;
        vec3d_t getPlanetocentricFromEcliptic(const vec3d_t &pos, double tdb) const;
        vec3d_t getHeliocentric(double jd) const;

    private:
        FrameTree frame;

        CelestialType bodyType = ctUnknown;

        PlanetarySystem *ownSystem = nullptr;
        PlanetarySystem *inSystem = nullptr;
    };
}
