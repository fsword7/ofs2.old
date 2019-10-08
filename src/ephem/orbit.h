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
	//	// Orbital elements
	//	double  T;  // Period [days]
	//	double  a;  // Semi-major Axis
	//	double  e;  // Eccentricity [rad]
	//	double  q;  // Perifocal distance
	//	double  i;  // Inclination [rad]
	//	double  Om; // Longitude of ascending node [rad]
	//	double  w;  // Argument of periapsis [rad]
	//	double  M0; // Mean anomaly at Epoch (time)
	//
	//	double  perigee;  // pericenter (perigee)
	//	double  epoch;
	//	double  GM;  // Standard gravitational parameter [GM]
	//
	//	quatd_t plane;
	};
}
