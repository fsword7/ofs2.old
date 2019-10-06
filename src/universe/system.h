/*
 * system.h - solar system package
 *
 *  Created on: Oct 6, 2019
 *      Author: Tim Stark
 */

#pragma once

namespace ofs::universe
{
	class System
	{
	public:
		System() = default;
		~System() = default;

		inline CelestialStar *getStar(int idx)
			{ return (idx < stars.size()) ? stars[idx] : nullptr; }

		void addObject(Object *object);

	private:
		vector<CelestialStar *> stars;
		PlanetarySystem *bodies = nullptr;
	//	SystemTree *systemTree  = nullptr;
	};
}
