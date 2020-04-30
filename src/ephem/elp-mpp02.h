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

		// parameters for adjusting the lunar and planetary arguments
		struct elpParams {
			// parameters adjusted to fit data
			double  Dw1_0, Dw2_0, Dw3_0, Deart_0, Dperi, Dw1_1, Dgam, De, Deart_1, Dep,
					Dw2_1, Dw3_1, Dw1_2, Dw1_3, Dw1_4, Dw2_2, Dw2_3, Dw3_2, Dw3_3;

			// parameters derived from the previous parameters
			double  Cw2_1, Cw3_1;
		};

		// Arguments for ELP/MPP02 series
		struct elpArgs {
			double W1, D, F, L, Lp, zeta, Me, Ve, EM, Ma, Ju, Sa, Ur, Ne;
		};

		// Factors multiplied by B1-B5 for longitude and latitude
		struct elpFACS {
		  double fA, fB1, fB2, fB3, fB4, fB5;
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

		elpParams params;
		elpArgs   args;
		elpFACS   facs;
	};

}
