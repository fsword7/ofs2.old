/*
 * starcolors.h
 *
 *  Created on: Jul 28, 2019
 *      Author: Tim Stark
 */

#pragma once

#define D58_nTEMP     0   // Surface temperature
#define D58_nK        1   // 'K'
#define D58_nCMF      2   // CIE 1921/CIE 1964
#define D58_nX        3   // Chromaticity Coordinates
#define D58_nY        4   // "
#define D58_nP        5   // Power in Semi-Arbitrary Units
#define D58_nFRED     6   // Red - Normalized
#define D58_nFGREEN   7   // Green - Normalized
#define D58_nFBLUE    8   // Blue - Normalized
#define D58_nIRED     9   // Red - Integer
#define D58_nIGREEN   10  // Green - Integer
#define D58_nIBLUE    11  // Blue - Integer
#define D58_nRGB      12  // RGB value
#define D58_nSIZE     13

class StarColors {
public:
	StarColors();
	~StarColors();

	bool load(std::string fname);

	Color lookup(int temp) const;

private:
	std::vector<Color> colors2;
	std::vector<Color> colors10;
	std::vector<int>   temps;
	int                minTemp, maxTemp;
	double             tempScale;
};
