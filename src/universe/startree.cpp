//
//  StarTreeBase.cpp
//  ofs
//
//  Created by Timothy Stark on 12/8/16.
//

#include "main/core.h"
#include "engine/object.h"
#include "universe/astro.h"
#include "universe/startree.h"
#include "universe/star.h"
//#include "render/scene.h"
//#include "render/stars.h"

using namespace ofs::astro;
using namespace ofs::universe;

StarTree::StarTree(const vec3d_t &cell, const double factor, StarTree *parent)
: Tree(parent), cellCenterPos(cell), exclusionFactor(factor)
{
	list.clear();
}

StarTree::~StarTree()
{
	delChildren();
}

double StarTree::decay(double factor)
{
	return convertLumToAbsMag(convertAbsMagToLum(factor) / 4.0);
}

void StarTree::add(const CelestialStar& obj)
{
    list.push_back(&obj);
}

void StarTree::insert(const CelestialStar& obj, double scale)
{
	StarTree *child;

	if (obj.getAbsMag() < exclusionFactor)
		add(obj);
	else if ((child = getChild(index(obj, cellCenterPos))) != nullptr)
		child->insert(obj, scale * 0.5);
	else {
		if (list.size() > STARTREE_THRESHOLD)
			split(scale * 0.5);
		add(obj);
	}
}

void StarTree::split(double scale)
{
	int count = 0;

	for (uint32_t idx = 0; idx < list.size(); idx++) {
		const CelestialStar& obj = *list[idx];

		if (obj.getAbsMag() < exclusionFactor)
			list[count++] = list[idx];
		else {
			vec3d_t   cell = cellCenterPos;
			uint32_t  idx  = index(obj, cell);
			StarTree *node = getChild(idx);

			if (node == nullptr) {
				cell += vec3d_t(((idx & xPos) != 0) ? scale : -scale,
								((idx & yPos) != 0) ? scale : -scale,
								((idx & zPos) != 0) ? scale : -scale);

				node = new StarTree(cell, decay(exclusionFactor), this);
				addChild(idx, node);
			}

			node->add(obj);
		}
	}

	list.resize(count);
}

uint32_t StarTree::index(const CelestialStar &obj, const vec3d_t &cell)
{
	vec3d_t  spos = obj.getPosition(0);

	return	((spos.x < cell.x) ? 0 : xPos) |
			((spos.y < cell.y) ? 0 : yPos) |
			((spos.z < cell.z) ? 0 : zPos);
}

//void StarTree::processVisibleStars(const ofsHandler &handle, const vec3d_t &obs,
//		const planed_t* frustum, const double limitingFactor,
//		const double scale)
//{
//
//	for (int idx = 0; idx < 5; idx++) {
//		const planed_t& plane = frustum[idx];
//		double r = scale * plane.normal().cwiseAbs().sum();
//		if (plane.signedDistance(cellCenterPos) < -r)
//			return;
//	}
//
//	double dist    = (obs - cellCenterPos).norm() - scale * sqrt(3.0);
//
//	for (uint32_t idx = 0; idx < list.size(); idx++) {
//		const CelestialStar& obj = *list[idx];
//
//		double dist = (obs - obj.getPosition(0)).norm();
//		double appMag = astro::convertAbsToAppMag(obj.getAbsMag(), dist);
//
//		handle.process(obj, dist, appMag);
//	}
//
//	if (dist <= 0 || astro::convertAbsToAppMag(exclusionFactor, dist) <= limitingFactor) {
//		for (int idx = 0; idx < 8; idx++) {
//			StarTree *node = getChild(idx);
//			if (node == nullptr)
//				continue;
//			node->processVisibleStars(handle, obs, frustum, limitingFactor, scale * 0.5);
//		}
//	}
//}

void StarTree::processNearStars(const vec3d_t &obs, const double radius, const double scale,
	vector<const CelestialStar *>& stars)
{

	double dist = glm::length(obs - cellCenterPos) - scale * sqrt(3.0);
	if (dist > radius)
		return;

	for (uint32_t idx = 0; idx < list.size(); idx++) {
		const CelestialStar *obj = list[idx];

		if (glm::length2(obs - obj->getPosition(0)) < square(radius)) {
			double dist = glm::length(obs - obj->getPosition(0));
//			double appMag = astro::convertAbsToAppMag(obj.getAbsMag(), dist);

			if (dist < radius)
				stars.push_back(obj);
		}
	}

	for (int idx = 0; idx < 8; idx++) {
		StarTree *node = getChild(idx);
		if (node == nullptr)
			continue;
		node->processNearStars(obs, radius, scale * 0.5, stars);
	}
}

uint32_t StarTree::countNodes()
{
	StarTree *child;
	uint32_t  count = 1;

	for (int idx = 0; idx < 8; idx++)
		if ((child = getChild(idx)) != nullptr)
			count += child->countNodes();

	return count;
}

uint32_t StarTree::countObjects()
{
	StarTree *child;
	uint32_t  count = list.size();

	for (int idx = 0; idx < 8; idx++)
		if ((child = getChild(idx)) != nullptr)
			count += child->countObjects();

	return count;
}
