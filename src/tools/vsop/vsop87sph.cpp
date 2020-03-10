/*
 * vsop87sph.cpp - VSOP87 C++ data generator package
 *
 * VSOP87 package are available for downloading:
 *
 * ftp://ftp.imcce.fr/pub/ephem/planets/vsop87/
 *
 *  Created on: Mar 10, 2020
 *      Author: Tim Stark
 */

#include <iostream>
#include <fstream>
#include <fmt/printf.h>

using namespace std;

//namespace fs = std::filesystem;

void usage(char *name)
{
	cout << fmt::sprintf("Usage: %s <planet name>\n", name);
	exit(1);
}

int main(int argc, char **argv)
{
	const char *planet = "earth"; // default planet name

	if (argc > 1)
		planet = argv[1];
	else
		usage(argv[0]);

	string fname = "VSOP87B." + string(planet, 0, 3);
//	cout << "Generating VSOP87 data terms from " << fname << " for " << planet << "..." << endl;

	ifstream vsopFile(fname, ios::in);
	if (!vsopFile.is_open()) {
		cerr << "File " << fname << ": " << strerror(errno) << endl;
		exit(1);
	}

	string line;
	int lnum = 0;

	while (getline(vsopFile, line)) {
		lnum++;
		const char *cline = line.c_str();
		if (!strncmp(cline, " VSOP87", 7)) {
//			cout << line << endl;
			int lbr = (int)cline[41] - (int)'1';
			int deg = (int)cline[59] - (int)'0';

//			cout << "LBR " << lbr << " Degree " << deg << endl;

			if (deg < 0 || deg > 5) {
				cout << "Bad degree (" << deg << ") in VSOP data file at line "
					 << lnum << endl;
				exit(1);
			}

			if (lbr != 0 || deg != 0)
				fmt::fprintf(cout, "\t{ %16.12lf, %16.12lf, %20.12lf }\n};\n\n",
					0.0, 0.0, 0.0);
			fmt::fprintf(cout, "vsop87_%s_%c%d = {\n",
				planet, "lbr"[lbr], deg);

		} else {

			double a, b, c;

			if (sscanf(cline+80, " %lf %lf %lf", &a, &b, &c) != 3) {
				cout << "Bad data in VSOP data at line " << lnum << endl;
				exit(1);
			}

			fmt::fprintf(cout, "\t{ %16.12lf, %16.12lf, %20.12lf },\n", a, b, c);
		}
	}

	fmt::fprintf(cout, "\t{ %16.12lf, %16.12lf, %20.12lf }\n};\n",
		0.0, 0.0, 0.0);

	vsopFile.close();
	exit(1);
}
