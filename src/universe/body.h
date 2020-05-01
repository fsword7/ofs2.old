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
			ctBarycenter,
			ctStar,
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

        inline bool hasSurface() const  { return flagSurface; }
        inline bool isClickable() const { return flagClickable; }
        inline bool isVisible() const   { return flagVisible; }

        inline void enableSurface()  { flagSurface = true; }
        inline void setInvisible()   { flagVisible = false; }

//        inline RotationModel *getRotationModel(double tjd) const { return rot; };

        quatd_t getEquatorial(double tjd) const;
        quatd_t getBodyFixed(double tjd) const;

        quatd_t getRotation(double tjd = 0) const override;

        double  getLuminosity(double luminosity, double distance) const;
        double  getApparentMagnitude(vec3d_t sun, double irradiance, vec3d_t view) const;

        vec3d_t getPlanetocentric(const vec3d_t &pos) const;
        vec3d_t getPlanetocentricFromEcliptic(const vec3d_t &pos, double tdb) const;
        vec3d_t getHeliocentric(double jd) const;

    private:
        FrameTree frame;

        // Celestial type parameter
        CelestialType bodyType = ctUnknown;

        // Celestial body flags parameters
        bool flagVisible = true;
        bool flagSurface = false;
        bool flagClickable = false;

        PlanetarySystem *ownSystem = nullptr;
        PlanetarySystem *inSystem = nullptr;
    };
}
