/*
 * star.cpp - Celestial star package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/astro.h"
#include "universe/star.h"

using namespace ofs::astro;
using namespace ofs::universe;

CelestialStar::CelestialStar()
: Object(objCelestialStar)
{

}

CelestialStar *CelestialStar::createSun()
{
  	CelestialStar *star = new CelestialStar();

	star->setName("Sol");
	star->ra       = 0.0;
	star->dec      = 0.0;
	star->plx      = 0.0;
	star->absMag   = SOLAR_ABSMAG;
	star->bMag     = 0.0;
	star->vMag     = 0.0;
	star->ci       = SOLAR_CI;
	star->lum      = SOLAR_LUMINOSITY;
	star->temp     = SOLAR_TEMPERATURE;

	star->flags    = knownRadius;
	star->radius   = SOLAR_RADIUS;

	star->hip      = 0;

	return star;
}

CelestialStar *CelestialStar::create(double ra, double dec, double plx,
    const char *spType, double appMag, double ci, double lum)
{
    CelestialStar *star;
    double pc;
    int    temp;

    star = new CelestialStar();
    pc = 1000.0 / plx;
    star->lpos = convertEquatorialToEcliptic(ra, dec, pc);
    star->objPosition = star->lpos;

    temp = (int)(4600 * (1/((ci*0.92)+1.7) + 1/((ci*0.92)+0.62)));

    star->ra  = ra;
    star->dec = dec;
    star->plx = plx;
    star->absMag = convertAppToAbsMag(appMag, pc);
    star->bMag = appMag + ci;
    star->vMag = appMag;
    star->ci  = ci;
    star->lum = lum;
    star->temp = temp;

    return star;
}
