/*
 * asterisms.cpp
 *
 *  Created on: Jul 29, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "universe/asterisms.h"


bool Constellations::load(std::string fname)
{
	std::ifstream  data(fname.c_str(), std::ios::in);
	std::string    line;
	int            lineno;
	int nAsterisms;

	Asterism *asterism;

	if (!data.is_open())
	{
		std::cerr << "I/O Error: file '" << fname << "': "
				  << std::strerror(errno) << std::endl;
		return false;
	}

	asterisms.clear();

	lineno = 0;
	nAsterisms = 0;
	while (std::getline(data, line))
	{
		std::stringstream lineStream(line);
		std::string       cell;
		std::vector<std::string> cells;

		if (line.empty())
			continue;
		if (line[0] == '#')
			continue;
		if (line[0] != '.')
			nAsterisms++;

		cells.clear();
		while(std::getline(lineStream, cell, ' '))
			cells.push_back(cell);

		asterism = new Asterism();
		asterism->shortName = cells[0];
		sscanf(cells[1].c_str(), "%d", &asterism->nLines);
		asterism->hip.clear();
		for (int idx = 2; idx < cells.size(); idx++) {
			int hip;

			if (cells[idx].empty())
				continue;
			sscanf(cells[idx].c_str(), "%d", &hip);
			asterism->hip.push_back(hip);
		}

		asterisms.push_back(asterism);
	}

	data.close();

	std::cout << "Total " << nAsterisms << " constellations" << std::endl;

//	for (int idx = 0; idx < nAsterisms; idx++) {
//		asterism = asterisms[idx];
//		std::cout << "Abbervated name: " << asterism->shortName
//				  << " Lines: " << asterism->nLines << std::endl;
//
//		std::cout << "HIP stars: ";
//		for (int ihip = 0; ihip < asterism->hip.size(); ihip++) {
//			if (ihip > 0)
//				std::cout << ",";
//			std::cout << asterism->hip[ihip];
//		}
//		std::cout << std::endl;
//	}

	return false;
}
