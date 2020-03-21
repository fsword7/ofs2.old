/*
 * vsop87.h - VSOP87 orbit empheremis package
 *
 *  Planetary emphemeris data for Sol solar system
 *
 *  Created on: Mar 15, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "ephem/orbit.h"

namespace ofs::ephem
{
	struct vsop87_t
	{
		double a, b, c;
	};

	struct vsop87s_t
	{
		vsop87s_t(vsop87_t *terms, int nTerms) : terms(terms), nTerms(nTerms)
		{
		}

		int nTerms;
		vsop87_t *terms;
	};

	class VSOP87Orbit : public CachingOrbit
	{
	public:
		VSOP87Orbit() = default;
		VSOP87Orbit(vsop87s_t *sL, int nL,
					vsop87s_t *sB, int nB,
					vsop87s_t *sR, int nR,
					double period);
		virtual ~VSOP87Orbit() = default;

		static Orbit *create(const string &name);

		vec3d_t calculatePosition(double jd) const override;
		vec3d_t calculateVelocity(double jd) const override;

	protected:
		double sum(const vsop87s_t &series, double t) const;

	protected:
		int        nL,  nB,  nR;
		vsop87s_t *sL, *sB, *sR;

		double period;
		double boundingRadius;
	};
}
