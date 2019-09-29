/*
 * asterisms.h
 *
 *  Created on: Jul 29, 2019
 *      Author: Tim Stark
 */

#pragma once

struct Asterism {
	std::string shortName;
	std::string fullName;
	int nLines;
	std::vector<int> hip;
};

class Constellations
{
public:
	Constellations() = default;
	~Constellations() = default;

	bool load(std::string fileName);

	const std::vector<Asterism *> &getAsterisms() const { return asterisms; }

private:
	std::vector<Asterism *> asterisms;
};
