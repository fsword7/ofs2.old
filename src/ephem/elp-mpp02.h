/*
 * elp-mpp02.h - ELP/MPP02 - Lunar Ephemeral data
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
		enum dataMode {
			elpUseLLR,
			elpUseDE406
		};

		ELP2000Orbit(dataMode mode);
		virtual ~ELP2000Orbit() = default;

//		static Orbit *create(const string &name);

		vec3d_t calculatePosition(double jd) const override;
//		vec3d_t calculateVelocity(double jd) const override;

		double getPeriod() const { return period; }
		double getBoundingRadius() const { return boundingRadius; }

	private:
		void setupParameters(dataMode mode);

	protected:
		double period;
		double boundingRadius;
	};

}
