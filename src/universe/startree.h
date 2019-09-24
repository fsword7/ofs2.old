//
//  startree.hpp
//  ofs
//
//  Created by Timothy Stark on 12/8/16.
//

#pragma once

#include "util/tree.h"

#define STARTREE_MAGNITUDE   6.0
#define STARTREE_ROOTSIZE    (10000000.0 / LY_PER_PARSEC)
#define STARTREE_THRESHOLD   75

namespace ofs::universe {
	class CelestialStar;

#include "universe/handler.h"

	class StarTree : public Tree<StarTree, OTREE_NODES>
	{
	public:
		enum {
			xPos = 1,
			yPos = 2,
			zPos = 4
		};

		StarTree(const vec3d_t &cell, const double factor, StarTree *parent = nullptr);
		~StarTree();

		void insert(const CelestialStar& obj, double scale);
		void add(const CelestialStar& obj);
		void split(double scale);

		uint32_t countNodes();
		uint32_t countObjects();

	    void processVisibleStars(const ofsHandler &handle, const vec3d_t &obs,
	    		/* const planed_t* frustum, */ const double limitingFactor,
	    		const double scale);

	    void processNearStars(const vec3d_t &obs, const double radius, const double scale,
	    		vector<const CelestialStar *>& stars);

	private:
		double   decay(double factor);
		uint32_t index(const CelestialStar& obj, const vec3d_t& cell);

		vec3d_t cellCenterPos;
		double  exclusionFactor;

		std::vector<const CelestialStar*> list;
	};

	//typedef OctreeHandler<CelestialStar, double> StarHandler;
}
