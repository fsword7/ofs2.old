/*
 * orbit.cpp
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "ephem/orbit.h"

using namespace ofs::astro;
using namespace ofs::ephem;

EllipticalOrbit::EllipticalOrbit(double _a, double _e, double _i,
		double _Om, double _w, double _M0, double _P, double mass, double ep)
: P(_P), a(_a), e(_e), q(0.0), i(_i), Om(_Om), w(_w), M0(_M0),
  perigee(0), epoch(ep), GM(0.0),
  plane(quatd_t(1,0,0,0))
{
	a = astro::convertAUToKilometer(a);
	perigee = a * (1 - e);

	// Compute the standard gravitational parameter
	GM = (astro::G * 1e-9) * mass;

	plane = zrot(Om) * xrot(i) * zrot(w);

//	cout << std::fixed << std::setprecision(10);
//	cout << "Period:                 " << P << endl;
//	cout << "Eccentricity:           " << e << endl;
//	cout << "Inclination:            " << glm::degrees(i) << endl;
//	cout << "Semimajor Axis:         " << a << endl;
//	cout << "Perigee:                " << perigee << endl;
//	cout << "Long of Ascending Node: " << glm::degrees(Om) << endl;
//	cout << "Arg of Pericenter:      " << glm::degrees(w) << endl;
//	cout << "Mean Anomaly (EPOCH):   " << glm::degrees(M0) << endl;
}

EllipticalOrbit::EllipticalOrbit(const vec3d_t &pos, const vec3d_t &vel, double mass, double t)
{
	GM = (astro::G * 1e-9) * mass;

	update(pos, vel, t);

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "Period:                 " << P << endl;
//	std::cout << "Eccentricity:           " << e << endl;
//	std::cout << "Inclination:            " << glm::degrees(i) << endl;
//	std::cout << "Semimajor Axis:         " << a << std::endl;
//	std::cout << "Perigee:                " << perigee << std::endl;
//	std::cout << "Long of Ascending Node: " << glm::degrees(Om) << endl;
//	std::cout << "Arg of Pericenter:      " << glm::degrees(w) << endl;
//	std::cout << "Mean Anomaly (EPOCH):   " << glm::degrees(M0) << endl;
}


// Find eccentric anomaly by using Kepler's equation
//
// Elliptic orbit			Hyperbolic orbit			Parabolic orbit
// f(E)  = E - e sin E		f(H)  = e sinh H - H		f(D)  = qD + D^3/6
// f'(E) = 1 - e cos E		f'(H) = e cosh H - 1		f'(D) = q + D^2/2
//
// Using Newton's equation
//
// E = f(E)/f'(E)
//
// Find accuracy of eccentric anomaly
//
// delta E = f(E)/f'(E) - M

double EllipticalOrbit::solveElliptic(double e, double M)
{
	const double accuracy = 1.0 / 1000000.0;
	double E = M;
	double dE;

	do {
		dE = (E - e * sin(E) - M) / (1 - e * cos(E));
		E -= dE;
	} while (dE >= accuracy);

	return E;
}

double EllipticalOrbit::solveParabolic(double e, double M)
{
	const double accuracy = 1.0 / 1000000.0;
	double D = M;
	double dD;

	do {
		dD = ((q*D) + ((D*D*D)/6.0) - M) / (q + ((D*D)/2.0));
		D -= dD;
	} while (dD < accuracy);

	return D;
}

double EllipticalOrbit::solveHyperbolic(double e, double M)
{
	const double accuracy = 1.0 / 1000000.0;
	double H = M;
	double dH;

	do {
		dH = (e * sinh(H) - H - M) / (e * cosh(H) - 1);
		H -= dH;
	} while (dH < accuracy);

	return H;
}

double EllipticalOrbit::solveForEccentricAnomaly(double M)
{
	if (e == 0) {
		// Circular orbit
		return M;
	} else if (e < 1.0) {
		// Elliptical orbit
		return solveElliptic(e, M);
	} else if (e == 1.0) {
		// Parabolic orbit (comets, etc.)
		return solveParabolic(e, M);
	} else {
		// Hyperbolic orbit
		return solveHyperbolic(e, M);
	}
}


vec3d_t EllipticalOrbit::getPositionAtE(double E)
{
	return vec3d_t(0, 0, 0);
}

//vec3d_t EllipticalOrbit::getPosition(double jd)
//{
//	return vec3d_t(0, 0, 0);
//}

//vec3d_t EllipticalOrbit::getVelocity(double jd)
//{
//	return vec3d_t(0, 0, 0);
//}

vec3d_t EllipticalOrbit::getPosition(double jd)
{
	double  meanMotion  = (PI * 2.0) / P;
	double  meanAnomaly = M0 + (jd - epoch) * meanMotion;
	double  E = solveForEccentricAnomaly(meanAnomaly);
	double  x, y;
	vec3d_t p;

	if (e < 1.0) {
		// Elliptical orbits
		x = a * (cos(E) - e);
		y = a * sqrt(1 - (e * e)) * sin(E);
	} else if (e > 1.0) {
		// Hyperbolic orbits
		x = -a * (e - cosh(E));
		y = -a * sqrt((e * e) - 1) * sinh(E);
	} else {
		// Parabolic orbits
//		x = q + (E*E)/2.0;
//		y = sqrt(2*q) * E;
		x = 0.0;
		y = 0.0;
	}

	p = plane * vec3d_t(x, y, 0);

	cout << fixed << setprecision(10) << "Orbit Position: JD "
		 << jd << " (" << p.x << "," << p.z << "," << -p.y << ")" << endl;

	return vec3d_t(p.x, p.z, -p.y);
}

vec3d_t EllipticalOrbit::getVelocity(double jd)
{
	double  meanMotion  = (PI * 2.0) / P;
	double  meanAnomaly = M0 + (jd - epoch) * meanMotion;
	double  E = solveForEccentricAnomaly(meanAnomaly);
	double  b, r, edot;
	double  x, y;
	vec3d_t v;

	if (e < 1.0) {
		// Elliptical orbits
		b = a * sqrt(1 - (e * e));
		r = a * (1 - e * cos(E));
		edot = (1 / r) * sqrt(GM / a);

		x = -a * sin(E) * edot;
		y =  b * cos(E) * edot;
	} else if (e > 1.0) {
		// Hyperbolic orbits
		b = -a * sqrt((e * e) - 1);
		r = -a * (e * cosh(E) - 1);
		edot = (1 / r) * sqrt(GM / -a);

		x = a * sinh(E) * edot;
		y = b * cosh(E) * edot;
	} else {
		// Parabolic orbits
//		r = q * (D*D)/2.0;
//		double Ddot = (1 / r) * sqrt(GM);
//
//		x = -E * Ddot;
//		y = sqrt(2*q) * Ddot;

		x = 0.0;
		y = 0.0;
	}

	v = plane * vec3d_t(x, y, 0);

//	std::cout << std::fixed << std::setprecision(10) << "Orbit Position: JD "
//			<< jd << " (" << p.x() << "," << p.z() << "," << -p.y() << ")" << std::endl;
	return vec3d_t(v.x, v.z, -v.y);
}

void EllipticalOrbit::update(const vec3d_t &pos, const vec3d_t &vel, double t)
{
	vec3d_t r     = vec3d_t(pos.x, -pos.z, pos.y);
	vec3d_t v     = vec3d_t(vel.x, -vel.z, vel.y);
	vec3d_t h     = glm::cross(r, v); // specific angular momentum (H = R x V)
	double  magr  = glm::length(r);   // magnitude of the position
	double  magv  = glm::length(v);   // magnitude of the velocity
	double  magh  = glm::length(h);   // magnitude of the specific angular momentum
	double  rv    = glm::dot(r, v) / magr;  // radical velocity
	vec3d_t ev;
	double  E;
	vec3d_t N;
	double  n;

	// Calculate the inclination
	i  = acos(h.z/magh);

	// Calculate the eccentricity
	ev = ((((magv*magv) / GM) - (1/magr)) * r) - ((rv / GM) * v);
	e  = glm::length(ev);

	// Calculate that semi-major axis
	a = 1.0 / (2.0 / magr - (magv*magv) / GM);

	// Calculate the perifocal distance for parabolic orbit
	q = ((magh * magh) / GM) / (1 + e);

	// Calculate the pericenter (perigee)
	perigee = a * (1.0 - e);

	// Calculate the eccentricity anomaly
	E = 2.0 * atan(tan(magv / 2.0) / sqrt((1.0 + e)/(1.0 - e)));
//	double  E = atan2(ev.y(), ev.x());

	// Calculate the mean anomaly
	M0 = E - e * sin(E);

	// Calculate the ascending node vector
	N = glm::cross(vec3d_t(0, 0, 1), h);
	n = glm::length(N);

	// Calculate the longitude of ascending node
	if (n != 0) {
		Om = acos(N.x / n);
		if (N.y < 0)
			Om = 2*PI - Om;
	} else
		Om = 0.0;
//	double  Om = atan2(h.x(), h.z());

	// Calculate the argument of periapsis
	if (n != 0 && e > 0) {
		w = acos(glm::dot(N, ev) / (n * e));
		if (ev.z < 0)
			w = 2*PI - w;
	} else
		w = 0.0;
//	vec3d_t U = r / magr;
//	vec3d_t W = h.cross(U);
//	double  p = (magh*magh) / GM;
//	double  snu = (v.dot(U) * sqrt(p / GM)) / e;
//	double  cnu = (v.dot(W) * sqrt(p / GM) - 1) / e;
//	vec3d_t P = U * cnu - W * snu;
//	vec3d_t Q = U * snu + W * cnu;
//	double  om = atan2(r.y(), Q.y());

	// Calculate the period in days
	P = (2*PI * sqrt((a*a*a) / GM)) / SECONDS_PER_DAY;
	epoch = t;

//	eccentricity = e;
//	inclination = i;
//	ascendingNode = Om;
//	argOfPeriapsis = om;
//	meanAnomalyEpoch = M;
//	pericenter = a - (1 - e);
//	period = p;
}

//EllipticalOrbit *EllipticalOrbit::create(vec3d_t &r, vec3d_t &v, double mass)
//{
//	vec3d_t h     = glm::cross(r, v); // specific angular momentum (H = R x V)
//	double  magr  = glm::length(r);   // magnitude of the position
//	double  magv  = glm::length(v);   // magnitude of the velocity
//	double  magh  = glm::length(h);   // magnitude of the specific angular momentum
//	double  rv    = glm::dot(r, v) / magr;  // radical velocity
//	double  GM    = (astro::G * 1e-9) * mass;
//
//	// Calculate that semi-major axis
//	double a = 1.0 / (2.0 / magr - (magv*magv) / GM);
//
//	// Calculate the inclination
//	double  i  = acos(h.z / magh);
//
//	// Calculate the eccentricity
//	vec3d_t ev = ((((magv*magv) / GM) - (1/magr)) * r) - ((rv / GM) * v);
//	double  e  = glm::length(ev);
//
//	// Calculate the eccentricity anomaly
//	double  E  = 2.0 * atan(tan(magv / 2.0) / sqrt((1.0 + e)/(1.0 - e)));
////	double  E = atan2(ev.y(), ev.x());
//
//	// Calculate the mean anomaly
//	double  M = E - e * sin(E);
//
//	// Calculate the ascending node vector
//	vec3d_t N = glm::cross(vec3d_t(0, 0, 1), h);
//	double  n = glm::length(N);
//
//	// Calculate the longitude of ascending node
//	double Om;
//	if (n != 0) {
//		Om = acos(N.x / n);
//		if (N.y < 0)
//			Om = 2*PI - Om;
//	} else
//		Om = 0.0;
////	double  Om = atan2(h.x(), h.z());
//
//	// Calculate the argument of periapsis
//	double w;
//	if (n != 0 && e > 0) {
//		w = acos(glm::dot(N, ev) / (n * e));
//		if (ev.z < 0)
//			w = 2*PI - w;
//	} else
//		w = 0.0;
////	vec3d_t U = r / magr;
////	vec3d_t W = h.cross(U);
////	double  p = (magh*magh) / GM;
////	double  snu = (v.dot(U) * sqrt(p / GM)) / e;
////	double  cnu = (v.dot(W) * sqrt(p / GM) - 1) / e;
////	vec3d_t P = U * cnu - W * snu;
////	vec3d_t Q = U * snu + W * cnu;
////	double  om = atan2(r.y(), Q.y());
//
//	// Calculate the period
//	double T = (2*PI * sqrt((a*a*a) / GM)) / 86400.0;
//
////	return EllipticalOrbit(e, i, Om, w, M, a * (1 - e), T);
//	return nullptr;
//}

// Determinating the orbital elements from state vector (position and velocity)
//
// state vector is relative to object center (reference frame)
//
// Bibliography:
//
// Orbital Mechanics for Engineers by Howard G. Curtis, 3rd edition, pp. 196-202
// Methods of Orbit Determination for the Microcomputer by Dan Boulet, pp. 149-157

//void EllipticalOrbit::update(const vec3d_t &r, const vec3d_t &v, double mass)
//{
//	vec3d_t h     = r.cross(v); // specific angular momentum (H = R x V)
//	double  magr  = r.norm();   // magnitude of the position
//	double  magv  = v.norm();   // magnitude of the velocity
//	double  magh  = h.norm();   // magnitude of the specific angular momentum
//	double  rv    = r.dot(v) / magr;  // radical velocity
//	double  GM    = (astro::G * 1e-9) * mass;
//
//	// Calculate the inclination
//	double  i  = acos(h.z()/magh);
//
//	// Calculate the ascending node vector
//	vec3d_t N = vec3d_t(0, 0, 1).cross(h);
//	double  n = N.norm();
//
//	// Calculate the longitude of ascending node
//	double Om;
//	if (n != 0) {
//		Om = acos(N.x()/n);
//		if (N.y() < 0)
//			Om = 2*PI - Om;
//	} else
//		Om = 0.0;
//
//	// Calculate the eccentricity
//	vec3d_t ev = ((((magv*magv) / GM) - (1/magr)) * r) - ((rv / GM) * v);
//	double  e  = ev.norm();
//
//	// Calculate the eccentricity anomaly
//	double  E  = 2.0 * atan(tan(magv / 2.0) / sqrt((1.0 + e)/(1.0 - e)));
//
//	// Calculate the mean anomaly
//	double  M  = E - e * sin(E);
//
//	// Calculate the argument of perigee
//	double w;
//	if (n != 0 && e > 1.e-10) {
//		w = acos(N.dot(ev) / (n * e));
//		if (ev.z() < 0)
//			w = 2*PI - w;
//	} else
//		w = 0.0;
//
//	// Calculate the true anomaly
//	double ta;
//	if (e > 1.e-10) {
//		ta = acos(ev.dot(r) / (e * magr));
//		if (rv < 0)
//			ta = 2*PI - ta;
//	} else {
//		ta = acos(N.dot(r) / (n * magr));
//		if (N.cross(r).y() >= 0)
//			ta = 2*PI - ta;
//	}
//
//	// Calculate that semi-major axis
//	double a = 1.0 / (2.0 / magr - (magv*magv) / GM);
//
//}
