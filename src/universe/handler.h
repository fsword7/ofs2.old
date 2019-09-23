/*
 * handler.h
 *
 *  Created on: Sep 23, 2019
 *      Author: Tim Stark
 */

#pragma once

class ofsHandler {
public:
	ofsHandler() = default;
	virtual ~ofsHandler() = default;

	virtual void process(const CelestialStar& star, double dist, double appMag) const = 0;
};

