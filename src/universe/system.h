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
	class SystemTree
	{
	public:
		SystemTree(Object *object);
		SystemTree(CelestialStar *star);
		SystemTree(CelestialBody *body);
		~SystemTree();

		inline CelestialStar *getStar() const { return starParent; }
		inline CelestialBody *getBody() const { return bodyParent; }
		inline int getSystemSize() const { return objects.size(); }

		inline bool isRoot() const { return bodyParent == nullptr; }

		void addObject(Object *object);

		Object *getObject(int idx) const;

	private:
		CelestialStar *starParent = nullptr;
		CelestialBody *bodyParent = nullptr;

		vector<Object *> objects;

		Frame *defaultFrame = nullptr;
	};

	class System
	{
	public:
		System(CelestialStar *star);
		~System() = default;

		inline CelestialStar *getStar(int idx)
			{ return (idx < stars.size()) ? stars[idx] : nullptr; }
		inline const SystemTree *getSystemTree() const { return &systemTree; }
		inline const PlanetarySystem *getCelestialBodies() const { return &objects; }

		void addObject(Object *object);

	private:
		SystemTree systemTree;
		PlanetarySystem objects;

		vector<CelestialStar *> stars;
	};
}
