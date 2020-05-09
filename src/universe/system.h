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
	class Universe;

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
		inline Frame *getDefaultReferenceFrame() const { return defaultFrame; }

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
		inline const PlanetarySystem *getPlanetarySystem() const { return &objects; }
		inline PlanetarySystem *getPlanetarySystem() { return &objects; }

		void addObject(Object *object);

		static CelestialBody *createBody(const string &nane, PlanetarySystem *system,
			CelestialType type, const string &orbitFrameName,
			const string &bodyFrameName);

	private:
		PlanetarySystem objects;
		SystemTree systemTree;

		vector<CelestialStar *> stars;
	};

	typedef map<uint32_t, System *> SystemCatalog;
}
