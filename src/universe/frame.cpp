/*
 * frame2.cpp - Reference frame package
 *
 *  Created on: Oct 17, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/body.h"
#include "universe/star.h"
#include "universe/frame.h"

using namespace ofs::universe;

// ******** Reference Frame Tree *********

FrameTree::FrameTree(CelestialStar *star)
: starParent(star), bodyParent(nullptr)
{
	defaultFrame = nullptr;
}

FrameTree::FrameTree(CelestialBody *body)
: starParent(nullptr), bodyParent(body)
{
	defaultFrame = nullptr;
}

// ******** Reference Frame ********

Frame::Frame(const Object *object)
: center(object)
{
}

int Frame::lock() const
{
	return ++refCount;
}

int Frame::release() const
{
	int count = --refCount;
	if (refCount <= 0)
		delete this;
	return count;
}


vec3d_t Frame::fromAstrocentric(const vec3d_t &upos, double tjd)
{
	if (center == nullptr)
		return upos;
	return getOrientation(tjd) * (center->getPosition(tjd) - upos);

}

vec3d_t Frame::toAstrocentric(const vec3d_t &lpos, double tjd)
{
	if (center == nullptr)
		return lpos;
	return center->getPosition(tjd) + (glm::conjugate(getOrientation(tjd)) * lpos);
}


vec3d_t Frame::fromUniversal(const vec3d_t &upos, double tjd)
{
	if (center == nullptr)
		return upos;
	return getOrientation(tjd) * (center->getPosition(tjd) - upos);
}

vec3d_t Frame::toUniversal(const vec3d_t &lpos, double tjd)
{
	if (center == nullptr)
		return lpos;
	return center->getPosition(tjd) + (glm::conjugate(getOrientation(tjd)) * lpos);
}


quatd_t Frame::fromUniversal(const quatd_t &urot, double tjd)
{
	if (center == nullptr)
		return urot;
	return urot * glm::conjugate(getOrientation(tjd));
}

quatd_t Frame::toUniversal(const quatd_t &lrot, double tjd)
{
	if (center == nullptr)
		return lrot;
	return lrot * getOrientation(tjd);
}


// ******** Player Reference Frame ********

PlayerFrame::PlayerFrame()
: type(csUniversal), frame(nullptr)
{
	frame = create(csUniversal);
}

PlayerFrame::PlayerFrame(coordType cs, const Object *obj)
: type(cs), frame(nullptr)
{
	frame = create(cs, obj);
}

PlayerFrame::~PlayerFrame()
{
	if (frame != nullptr)
		frame->release();
}

string PlayerFrame::getName() const
{
	return frame->getCenter()->getName();
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

Frame *PlayerFrame::create(coordType csType, const Object *obj)
{
	switch (csType) {
	case csUniversal:
		return new J2000EclipticFrame(nullptr);
	case csEcliptical:
		return new J2000EclipticFrame(obj);
	case csEquatorial:
	default:
		return new J2000EclipticFrame(nullptr);
	}
	return nullptr;
}


// ******** J2000 Earth Elliptic Reference Frame ********

J2000EclipticFrame::J2000EclipticFrame(const Object *obj)
: Frame(obj)
{
}

// ******** J2000 Earth Equator Reference Frame ********

J2000EquatorFrame::J2000EquatorFrame(const Object *obj, const Object *tgt)
: Frame(obj)
{
}

// ******** Body Fixed Reference Frame ********

BodyFixedFrame::BodyFixedFrame(const Object *obj, const Object *tgt)
: Frame(obj)
{
}

// ******** Body Mean Equator Reference Frame ********

BodyMeanEquatorFrame::BodyMeanEquatorFrame(const Object *obj, const Object *tgt)
: Frame(obj), equatorObject(tgt)
{
	cout << "Yes Body Mean Equator here." << endl;
}

quatd_t BodyMeanEquatorFrame::getOrientation(double tjd) const
{
	quatd_t q;

	cout << "Yes rotation frame here." << endl;

	switch (equatorObject->getType()) {
	case objCelestialBody:
		q = dynamic_cast<const CelestialBody *>(equatorObject)->getEclipticToEquatorial(tjd);
		cout << "Rotation Frame: " << q.w << "," << q.x << "," << q.y << "," << q.z << endl;
		return q;
	default:
		return quatd_t(1, 0, 0, 0);
	}
}
