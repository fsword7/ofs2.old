/*
 * frame.cpp - reference frame package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"

using namespace ofs::universe;

// ******** Reference Frame ********

ReferenceFrame::ReferenceFrame(const Object *object)
: center(object)
{
}

int ReferenceFrame::lock() const
{
	return ++refCount;
}

int ReferenceFrame::release() const
{
	int count = --refCount;
	if (refCount <= 0)
		delete this;
	return count;
}

vec3d_t ReferenceFrame::fromUniversal(const vec3d_t& upos, double tjd)
{
//	vec3d_t pos;

	if (center == nullptr)
		return upos;
	return glm::conjugate(getRotation(tjd)) * (center->getPosition(tjd) - upos);

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "Time: (from) " << tjd << " (" << pos.x() << "," << pos.y() << "," << pos.z() << ")" << std::endl;
//	return pos;
}

vec3d_t ReferenceFrame::toUniversal(const vec3d_t& lpos, double tjd)
{
//	vec3d_t pos;

	if (center == nullptr)
		return lpos;
	return center->getPosition(tjd) + (getRotation(tjd) * lpos);

//	pos = center->getPosition(tjd) + lpos;
//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "Time: (to) " << tjd << " (" << pos.x() << "," << pos.y() << "," << pos.z() << ")" << std::endl;
//	std::cout << "Time:      " << tjd << " (" << lpos.x() << "," << lpos.y() << "," << lpos.z() << ")" << std::endl;
//	return pos;
}

quatd_t ReferenceFrame::fromUniversal(const quatd_t& urot, double tjd)
{
	if (center == nullptr)
		return urot;
	return urot * glm::conjugate(getRotation(tjd));
}

quatd_t ReferenceFrame::toUniversal(const quatd_t& lrot, double tjd)
{
	if (center == nullptr)
		return lrot;
	return lrot * getRotation(tjd);
}

// ******** Player Frame ********

PlayerFrame::PlayerFrame()
: csType(csUniversal), frame(nullptr)
{
	frame = create(csUniversal);
}

PlayerFrame::PlayerFrame(CoordType cs, const Object *obj)
: csType(cs), frame(nullptr)
{
	frame = create(cs, obj);
}

PlayerFrame::~PlayerFrame()
{
	if (frame != nullptr)
		frame->release();
}

vec3d_t PlayerFrame::fromUniversal(const vec3d_t& pos, double tjd)
{
	return frame->fromUniversal(pos, tjd);
}

quatd_t PlayerFrame::fromUniversal(const quatd_t& rot, double tjd)
{
	return frame->fromUniversal(rot, tjd);
}

vec3d_t PlayerFrame::toUniversal(const vec3d_t& pos, double tjd)
{
	return frame->toUniversal(pos, tjd);
}

quatd_t PlayerFrame::toUniversal(const quatd_t& rot, double tjd)
{
	return frame->toUniversal(rot, tjd);
}

ReferenceFrame *PlayerFrame::create(CoordType cs, const Object *obj)
{
	switch (cs) {
	case csUniversal:
		return new EclipticFrame(nullptr);
	case csEcliptical:
		return new EclipticFrame(obj);
	case csEquatorial:
	default:
		return new EclipticFrame(nullptr);
	}
}

std::string PlayerFrame::name() const
{
	return frame->getCenter()->getName();
}

// **** **** Elliptic Reference Frame ********

EclipticFrame::EclipticFrame(const Object *obj)
: ReferenceFrame(obj)
{
}

// ******** Equator Reference Frame ********

EquatorFrame::EquatorFrame(const Object *obj, const Object *tgt)
: ReferenceFrame(obj)
{
}

// ******** Body Mean Reference Frame ********

BodyMeanFrame::BodyMeanFrame(const Object *obj, const Object *tgt)
: ReferenceFrame(obj)
{
}
