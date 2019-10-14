/*
 * astro.h - OFS Astronomy formula package
 *
 *  Created on: Oct 2, 2018
 *      Author: Tim Stark
 */

#pragma once

#include "main/date.h"

#define LN_MAG          1.085736
#define J2000Obliquity  glm::radians(23.4392911)

#define LY_PER_PARSEC  3.26167
#define KM_PER_LY      9460730472580.8
#define AU_PER_PC      206264.81
#define KM_PER_AU      149597870.7
#define AU_PER_LY      (KM_PER_LY / KM_PER_AU)
#define KM_PER_PC      (KM_PER_AU * AU_PER_PC)

#define EQUATIORIAL_TO_ECLIPTIC_ROT quatd_t(AngleAxis<double>(-J2000Obliquity), vec3d_t::UnitX())

// default solar parameters (our sun)
#define SOLAR_ABSMAG		4.83
#define SOLAR_IRRADIANCE	1367.6
#define SOLAR_POWER			3.8462e26
#define SOLAR_LUMINOSITY	1.0
#define SOLAR_TEMPERATURE	5777.0
#define SOLAR_CI			0.656
#define SOLAR_RADIUS		696000.0
#define SOLAR_MASS			1.9885e30

namespace ofs::astro {
	constexpr double G = 6.673889e-11;  // Official gravitational constant [N (m/kg)^2]
	constexpr double J2000 = 2451545.0; // Epoch J2000 [Jan 1, 2000 at 12:00 UTC]

	vec3d_t convertEquatorialToEcliptic(double ra, double dec, double pc);

	template <class T> T convertParsecToKilometer(T pc)
	{
		return pc * (T)KM_PER_PC;
	}

	template <class T> T convertKilometerToAU(T km)
	{
		return km / (T)KM_PER_AU;
	}

	template <class T> T convertAUToKilometer(T au)
	{
		return au * (T)KM_PER_AU;
	}

	template <class T> T convertKilometerToParsec(T km)
	{
		return km / (T)KM_PER_PC;
	}



	template <class T> T convertAbsToAppMag(T absMag, T pc)
	{
		return absMag - 5 + 5 * log10(pc);
	}

	template <class T> T convertAppToAbsMag(T appMag, T pc)
	{
		return appMag + 5 - 5 * log10(pc);
	}

	template <class T> T convertAbsMagToLum(T absMag)
	{
		return exp((SOLAR_ABSMAG - absMag) / LN_MAG);
	}

	template <class T> T convertAppMagToLum(const T mag, const T pc)
	{
		return convertAbsMagToLum(convertAppToAbsMag(mag, pc));
	}

	template <class T> T convertLumToAbsMag(const T mag)
	{
		return (SOLAR_ABSMAG - log(mag) * LN_MAG);
	}

	template <class T> T convertLumToAppMag(const T mag, const T pc)
	{
		return convertAbsToAppMag(convertLumToAbsMag(mag), pc);
	}



	// Calculate the velocity for circular orbit [km/s]
	template <class T> T getOrbitalVelocity(const T mass, const T radius)
	{
		return sqrt((G * mass * 1e-9) / radius);
	}

	// Calculate the velocity for escaping from gravity [km/s]
	template <class T> T getEscapeVelocity(const T mass, const T radius)
	{
		return sqrt((T(2.0) * G * mass * 1e-9) / radius);
	}
}
