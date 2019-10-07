/*
 * system.h - solar system package
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"

namespace ofs::universe
{
	class CelestialStar;

	class System
	{
	public:
		System(CelestialStar *star);
		~System() = default;

		inline CelestialStar *getStar(int idx)
			{ return (idx < stars.size()) ? stars[idx] : nullptr; }
		inline const SystemTree *getSystemTree() const { return &systemTree; }

		void addObject(Object *object);

	private:
		SystemTree systemTree;
		PlanetarySystem objects;

		vector<CelestialStar *> stars;
	};
}
