/*
 * orbit.h
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#pragma once

namespace ofs::ephem
{
	class Orbit
	{
	public:
		Orbit() = default;
		virtual ~Orbit() = default;

		virtual vec3d_t getPosition(double jd) = 0;
		virtual vec3d_t getVelocity(double jd) = 0;
	};

	class EllipticalOrbit : public Orbit
	{
	public:
		EllipticalOrbit() = default;
		virtual ~EllipticalOrbit() = default;

		vec3d_t getPosition(double jd) override;
		vec3d_t getVelocity(double jd) override;

	protected:
	};

//	class EllipticalOrbit : public Orbit
//	{
//	public:
//		EllipticalOrbit(double a, double e, double i, double Om, double w, double M0,
//				double T, double mass, double epoch = 2451545.0);
//		EllipticalOrbit(const vec3d_t &pos, const vec3d_t &vel, double mass, double t);
//		EllipticalOrbit();
//		virtual ~EllipticalOrbit();
//
//		// Kepler's equation
//		double solveElliptic(double e, double M);
//		double solveParabolic(double e, double M);
//		double solveHyperbolic(double e, double M);
//
//		double solveForEccentricAnomaly(double M);
//		vec3d_t positionAtE(double E);
//
//		virtual vec3d_t position(double jd);
//		virtual vec3d_t velocity(double jd);
//
//		double getPeriod() const { return T; }
//
//		// Determine orbit path by using position, velocity and mass.
//		static EllipticalOrbit *create(vec3d_t &p, vec3d_t &v, double mass);
//
//		// Determine new orbit path by using changing position and velocity
//		// for rocket launch, orbit transfer, etc.
//		void update(const vec3d_t &pos, const vec3d_t &val, double t);
//
//	private:
//		// Orbital elements
//		double  T;  // Period [days]
//		double  a;  // Semi-major Axis
//		double  e;  // Eccentricity [rad]
//		double  q;  // Perifocal distance
//		double  i;  // Inclination [rad]
//		double  Om; // Longitude of ascending node [rad]
//		double  w;  // Argument of periapsis [rad]
//		double  M0; // Mean anomaly at Epoch (time)
//
//		double  perigee;  // pericenter (perigee)
//		double  epoch;
//		double  GM;  // Standard gravitational parameter [GM]
//
//		quatd_t plane;
//	};

}
