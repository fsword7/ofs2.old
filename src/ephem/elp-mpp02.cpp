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
	setupCoefficients();
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

void ELP2000Orbit::loadMainProblemFile(const char *fname,
	int &n, int **&iMain, double *&aMain, double fA, elpFACS &facs)
{
	double A, B1, B2, B3, B4, B5, B6;
	ifstream file(fname, ios::in);

	if (!file) {
		cerr << fmt::sprintf("ELP2000Orbit: File '%s': %s\n",
				fname, strerror(errno));
		return;
	}

	file >> n;

	iMain = new int *[n];
	aMain = new double[n];
	for (int idx = 0; idx < n; idx++)
		iMain[idx] = new int[4];

	for (int idx = 0; idx < n; idx++) {
		if (file.eof()) {
			cerr << fmt::sprintf("ELP2000Orbit: File '%s': reached EOF before reading all data!\n",
					fname);
			return;
		}

		file >> iMain[idx][0] >> iMain[idx][1] >> iMain[idx][2] >> iMain[idx][3]
			 >> A >> B1 >> B2 >> B3 >> B4 >> B5 >> B6;
		aMain[idx] = (fA * A) + (facs.fB1 * B1) + (facs.fB2 * B2) + (facs.fB3 * B3) +
			(facs.fB4 * B4) + (facs.fB5 * B5);
	}

	file.close();
}

void ELP2000Orbit::loadPerturbationFile(const char *fname,
	int &n, int **&iPert, double *&aPert, double *&phase)
{
	ifstream file(fname, ios::in);

	if (!file) {
		cerr << fmt::sprintf("ELP2000Orbit: File '%s': %s\n",
				fname, strerror(errno));
		return;
	}

	file >> n;

	iPert = new int *[n];
	aPert = new double[n];
	phase = new double[n];
	for (int idx = 0; idx < n; idx++)
		iPert[idx] = new int[13];

	for (int idx = 0; idx < n; idx++) {
		if (file.eof()) {
			cerr << fmt::sprintf("ELP2000Orbit: File '%s': reached EOF before reading all data!\n",
					fname);
			return;
		}

		file >> iPert[idx][0]  >> iPert[idx][1] >> iPert[idx][2] >> iPert[idx][3]
			 >> iPert[idx][4]  >> iPert[idx][5] >> iPert[idx][6] >> iPert[idx][7]
			 >> iPert[idx][8]  >> iPert[idx][9] >> iPert[idx][10] >> iPert[idx][11]
			 >> iPert[idx][12] >> aPert[idx] >> phase[idx];
	}

	file.close();
}

void ELP2000Orbit::setupCoefficients()
{
	  string infile;

	  // Main problem
	  infile = "data/ephem/elp_main.long";
	  loadMainProblemFile(infile.c_str(), coefs.n_main_long, coefs.i_main_long,
	                         coefs.A_main_long, 1.0, facs);
	  infile = "data/ephem/elp_main.lat";
	  loadMainProblemFile(infile.c_str(), coefs.n_main_lat, coefs.i_main_lat,
	                         coefs.A_main_lat, 1.0, facs);
	  infile = "data/ephem/elp_main.dist";
	  loadMainProblemFile(infile.c_str(), coefs.n_main_dist, coefs.i_main_dist,
	                         coefs.A_main_dist, facs.fA, facs);

	  // perturbation, longitude
	  infile = "data/ephem/elp_pert.longT0";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_longT0, coefs.i_pert_longT0,
	                         coefs.A_pert_longT0, coefs.ph_pert_longT0);
	  infile = "data/ephem/elp_pert.longT1";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_longT1, coefs.i_pert_longT1,
	                         coefs.A_pert_longT1, coefs.ph_pert_longT1);
	  infile = "data/ephem/elp_pert.longT2";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_longT2, coefs.i_pert_longT2,
	                         coefs.A_pert_longT2, coefs.ph_pert_longT2);
	  infile = "data/ephem/elp_pert.longT3";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_longT3, coefs.i_pert_longT3,
	                         coefs.A_pert_longT3, coefs.ph_pert_longT3);

	  // perturbation, latitude
	  infile = "data/ephem/elp_pert.latT0";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_latT0, coefs.i_pert_latT0,
	                         coefs.A_pert_latT0, coefs.ph_pert_latT0);
	  infile = "data/ephem/elp_pert.latT1";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_latT1, coefs.i_pert_latT1,
	                         coefs.A_pert_latT1, coefs.ph_pert_latT1);
	  infile = "data/ephem/elp_pert.latT2";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_latT2, coefs.i_pert_latT2,
	                         coefs.A_pert_latT2, coefs.ph_pert_latT2);

	  // perturbation, distance
	  infile = "data/ephem/elp_pert.distT0";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_distT0, coefs.i_pert_distT0,
	                         coefs.A_pert_distT0, coefs.ph_pert_distT0);
	  infile = "data/ephem/elp_pert.distT1";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_distT1, coefs.i_pert_distT1,
	                         coefs.A_pert_distT1, coefs.ph_pert_distT1);
	  infile = "data/ephem/elp_pert.distT2";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_distT2, coefs.i_pert_distT2,
	                         coefs.A_pert_distT2, coefs.ph_pert_distT2);
	  infile = "data/ephem/elp_pert.distT3";
	  loadPerturbationFile(infile.c_str(), coefs.n_pert_distT3, coefs.i_pert_distT3,
	                         coefs.A_pert_distT3, coefs.ph_pert_distT3);

}

vec3d_t ELP2000Orbit::calculatePosition(double jd) const
{
	// Julian time since EPOCH J2000.0
	double t = (jd - 2451545.0) / 36525.0;

	return vec3d_t(0, 0, 0);
}



