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

		virtual vec3d_t getPosition(double jd) const = 0;
		virtual vec3d_t getVelocity(double jd) const = 0;

		virtual bool   isPeriodic() const { return true; }
		virtual double getPeriod() const = 0;
		virtual double getBoundingRadius() const = 0;
	};

	class CachingOrbit : public Orbit
	{
	public:
		CachingOrbit() = default;
		virtual ~CachingOrbit() = default;

		vec3d_t getPosition(double jd) const override;
		vec3d_t getVelocity(double jd) const override;

		virtual vec3d_t calculatePosition(double jd) const = 0;
		virtual vec3d_t calculateVelocity(double jd) const;

	private:
		// Must use 'mutable' to allow variables modifiable in const objects.
		mutable double  lastTime = -numeric_limits<double>::infinity();
		mutable vec3d_t lastPosition = vec3d_t(0.0, 0.0, 0.0);
		mutable vec3d_t lastVelocity = vec3d_t(0.0, 0.0, 0.0);

		mutable bool positionValid = false;
		mutable bool velocityValid = false;
	};

	class EllipticalOrbit : public Orbit
	{
	public:
		EllipticalOrbit() = default;
		EllipticalOrbit(double a, double e, double i, double Om, double w, double M0,
				double T, double mass, double epoch = 2451545.0);
		EllipticalOrbit(const vec3d_t &pos, const vec3d_t &vel, double mass, double t);
		virtual ~EllipticalOrbit() = default;

		inline double getPeriod() const { return P; }
		inline double getBoundingRadius() const { return 0.0; }

		// Kepler's equation
		double solveElliptic(double e, double M) const;
		double solveParabolic(double e, double M) const;
		double solveHyperbolic(double e, double M) const;

		double solveForEccentricAnomaly(double M) const;
		vec3d_t getPositionAtE(double E);

		vec3d_t getPosition(double jd) const override;
		vec3d_t getVelocity(double jd) const override;

		// Determine new orbit path by using changing position and velocity
		// for rocket launch, orbit transfer, etc.
		void update(const vec3d_t &pos, const vec3d_t &val, double t);

		// Determine orbit path by using position, velocity and mass.
//		static EllipticalOrbit *create(vec3d_t &p, vec3d_t &v, double mass);

	protected:
		// Orbital elements
		double  P;  // Period [days]
		double  a;  // Semi-major Axis
		double  e;  // Eccentricity [rad]
		double  q;  // Perifocal distance
		double  i;  // Inclination [rad]
		double  Om; // Longitude of ascending node [rad]
		double  w;  // Argument of periapsis [rad]
		double  M0; // Mean anomaly at Epoch (time)

		double  perigee;  // pericenter (perigee)
		double  epoch;
		double  GM;  // Standard gravitational parameter [GM]

		quatd_t plane;
	};
}
