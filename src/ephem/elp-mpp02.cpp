/*
 * elp-mpp02.cpp - ELP/MPP02 Lunar ephemeral data
 *
 *  Created on: Mar 23, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "ephem/elp-mpp02.h"
#include "universe/astro.h"

using namespace ofs::ephem;

ELP2000Orbit::ELP2000Orbit(dataMode mode)
{
	setupParameters(mode);
}

void ELP2000Orbit::setupParameters(dataMode mode)
{

	switch (mode)
	{
	case elpUseLLR:
        params.Dw1_0   = -0.10525;
        params.Dw2_0   =  0.16826;
        params.Dw3_0   = -0.10760;
        params.Deart_0 = -0.04012;
        params.Dperi   = -0.04854;
        params.Dw1_1   = -0.32311;
        params.Dgam    =  0.00069;
        params.De      =  0.00005;
        params.Deart_1 =  0.01442;
        params.Dep     =  0.00226;
        params.Dw2_1   =  0.08017;
        params.Dw3_1   = -0.04317;
        params.Dw1_2   = -0.03794;
        params.Dw1_3   =  0.0;
        params.Dw1_4   =  0.0;
        params.Dw2_2   =  0.0;
        params.Dw2_3   =  0.0;
        params.Dw3_2   =  0.0;
        params.Dw3_3   =  0.0;
		break;

	case elpUseDE406:
        params.Dw1_0   = -0.07008;
        params.Dw2_0   =  0.20794;
        params.Dw3_0   = -0.07215;
        params.Deart_0 = -0.00033;
        params.Dperi   = -0.00749;
        params.Dw1_1   = -0.35106;
        params.Dgam    =  0.00085;
        params.De      = -0.00006;
        params.Deart_1 =  0.00732;
        params.Dep     =  0.00224;
        params.Dw2_1   =  0.08017;
        params.Dw3_1   = -0.04317;
        params.Dw1_2   = -0.03743;
        params.Dw1_3   = -0.00018865;
        params.Dw1_4   = -0.00001024;
        params.Dw2_2   =  0.00470602;
        params.Dw2_3   = -0.00025213;
        params.Dw3_2   = -0.00261070;
        params.Dw3_3   = -0.00010712;
		break;
	}

	// derived parameters
	const double am = 0.074801329;
	const double alpha = 0.002571881;
	const double dtsm = 2.0 * alpha / ( 3.0 * am);
	const double xa = 2.0 * alpha / 3.0;
	const double sec = PI / 648000.0; // arcsecs -> radians

	double bp[5][2] = {
		{ 0.311079095, -0.103837907},
		{-0.004482398,  0.000668287},
		{-0.001102485, -0.001298072},
		{ 0.001056062, -0.000178028},
		{ 0.000050928, -0.000037342}
	};

	double w11 = (1732559343.73604 + params.Dw1_1)*sec;
	double w21 = (14643420.3171 + params.Dw2_1)*sec;
	double w31 = (-6967919.5383 + params.Dw3_1)*sec;
	double x2 = w21/w11;
	double x3 = w31/w11;
	double y2 = am*bp[0][0] + xa*bp[4][0];
	double y3 = am*bp[0][1] + xa*bp[4][1];
	double d21 = x2-y2;
	double d22 = w11*bp[1][0];
	double d23 = w11*bp[2][0];
	double d24 = w11*bp[3][0];
	double d25 = y2/am;
	double d31 = x3-y3;
	double d32 = w11*bp[1][1];
	double d33 = w11*bp[2][1];
	double d34 = w11*bp[3][1];
	double d35 = y3/am;

	params.Cw2_1 = d21*params.Dw1_1 + d25*params.Deart_1 + d22*params.Dgam +
				   d23*params.De + d24*params.Dep;
	params.Cw3_1 = d31*params.Dw1_1 + d35*params.Deart_1 + d32*params.Dgam +
				   d33*params.De + d34*params.Dep;

	// factors multipled by B1-B5 for longitude and latitude
	double delnu_nu = (0.55604 + params.Dw1_1)*sec/w11;
	double dele = (0.01789 + params.De)*sec;
	double delg = (-0.08066 + params.Dgam)*sec;
	double delnp_nu = (-0.06424 + params.Deart_1)*sec/w11;
	double delep = (-0.12879 + params.Dep)*sec;

	// factors multipled by B1-B5 for longitude and latitude
	facs.fB1 = -am*delnu_nu + delnp_nu;
	facs.fB2 = delg;
	facs.fB3 = dele;
	facs.fB4 = delep;
	facs.fB5 = -xa*delnu_nu + dtsm*delnp_nu;
	// factor multiplie A_i for distance
	facs.fA = 1.0 - 2.0/3.0*delnu_nu;

}

vec3d_t ELP2000Orbit::calculatePosition(double jd) const
{
	// Julian time since EPOCH J2000.0
	double t = (jd - 2451545.0) / 36525.0;

	return vec3d_t(0, 0, 0);
}



