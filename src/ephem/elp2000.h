/*
 * elp2000.h - ELP 2000/82 - Lunar Ephemeris orbital data
 *
 *  Created on: Mar 23, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "ephem/orbit.h"

namespace ofs::ephem {

	class ELP2000Orbit : public CachingOrbit
	{
	public:
		ELP2000Orbit() = default;
		virtual ~ELP2000Orbit() = default;

//		static Orbit *create(const string &name);

		vec3d_t calculatePosition(double jd) const override;
//		vec3d_t calculateVelocity(double jd) const override;

		double getPeriod() const { return period; }
		double getBoundingRadius() const { return boundingRadius; }

	protected:
		double period;
		double boundingRadius;
	};

}
