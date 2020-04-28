/*
 * frame.cpp - Reference frame package
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
	defaultFrame = new J2000EclipticFrame(star);
}

FrameTree::FrameTree(CelestialBody *body)
: starParent(nullptr), bodyParent(body)
{
	defaultFrame = new BodyMeanEquatorFrame(body, body);
}

FrameTree::~FrameTree()
{
	if (defaultFrame != nullptr)
		defaultFrame->release();
}

void FrameTree::addObject(Object *object)
{
	object->setOrbitFrame(defaultFrame);
	objects.push_back(object);
}

Object *FrameTree::getObject(int idx) const
{
	if (idx < 0)
		return nullptr;
	if (idx <= objects.size())
		return objects[idx];
	return nullptr;
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

	vec3d_t opos = center->getPosition(tjd);
	quatd_t orot = glm::conjugate(getOrientation(tjd));
	vec3d_t rpos = (opos - upos) * orot;

//	cout << fmt::sprintf("Center: P(%lf,%lf,%lf) Q(%lf,%lf,%lf,%lf)\n",
//		opos.x, opos.y, opos.z, orot.w, orot.x, orot.y, orot.z);
//	cout << fmt::sprintf(" Frame: U(%lf,%lf,%lf) => L(%lf,%lf,%lf)\n",
//		upos.x, upos.y, upos.z, rpos.x, rpos.y, rpos.z);

	return rpos;
}

vec3d_t Frame::toAstrocentric(const vec3d_t &lpos, double tjd)
{
	if (center == nullptr)
		return lpos;

	vec3d_t opos = center->getPosition(tjd);
	quatd_t orot = getOrientation(tjd);
	vec3d_t rpos = opos + (lpos * orot);

//	cout << fmt::sprintf("Center: P(%lf,%lf,%lf) Q(%lf,%lf,%lf,%lf)\n",
//		opos.x, opos.y, opos.z, orot.w, orot.x, orot.y, orot.z);
//	cout << fmt::sprintf(" Frame: L(%lf,%lf,%lf) => U(%lf,%lf,%lf)\n",
//		lpos.x, lpos.y, lpos.z, rpos.x, rpos.y, rpos.z);

	return rpos;
}


vec3d_t Frame::fromUniversal(const vec3d_t &upos, double tjd)
{
	if (center == nullptr)
		return upos;

	vec3d_t opos = center->getPosition(tjd);
	quatd_t orot = glm::conjugate(getOrientation(tjd));
	vec3d_t rpos = (opos - upos) * orot;

//	cout << fmt::sprintf("Center: P(%lf,%lf,%lf) Q(%lf,%lf,%lf,%lf)\n",
//		opos.x, opos.y, opos.z, orot.w, orot.x, orot.y, orot.z);
//	cout << fmt::sprintf(" Frame: U(%lf,%lf,%lf) => L(%lf,%lf,%lf)\n",
//		upos.x, upos.y, upos.z, rpos.x, rpos.y, rpos.z);

	return rpos;
}

vec3d_t Frame::toUniversal(const vec3d_t &lpos, double tjd)
{
	if (center == nullptr)
		return lpos;

	vec3d_t opos = center->getPosition(tjd);
	quatd_t orot = getOrientation(tjd);
	vec3d_t rpos = opos + (lpos * orot);

//	cout << fmt::sprintf("Center: P(%lf,%lf,%lf) Q(%lf,%lf,%lf,%lf)\n",
//		opos.x, opos.y, opos.z, orot.w, orot.x, orot.y, orot.z);
//	cout << fmt::sprintf(" Frame: L(%lf,%lf,%lf) => U(%lf,%lf,%lf)\n",
//		lpos.x, lpos.y, lpos.z, rpos.x, rpos.y, rpos.z);

	return rpos;
}


quatd_t Frame::fromUniversal(const quatd_t &urot, double tjd)
{
	if (center == nullptr)
		return urot;
	return urot * getOrientation(tjd);
}

quatd_t Frame::toUniversal(const quatd_t &lrot, double tjd)
{
	if (center == nullptr)
		return lrot;
	return glm::conjugate(getOrientation(tjd)) * lrot;
}


// ******** Player Reference Frame ********

PlayerFrame::PlayerFrame()
: type(csUniversal), frame(nullptr)
{
	frame = create(csUniversal);
}

PlayerFrame::PlayerFrame(coordType cs, const Object *obj, const Object *target)
: type(cs), frame(nullptr)
{
	frame = create(cs, obj, target);
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

Frame *PlayerFrame::create(coordType csType, const Object *obj, const Object *target)
{
	switch (csType) {
	case csUniversal:
		return new J2000EclipticFrame(nullptr);
	case csEcliptical:
		return new J2000EclipticFrame(obj);
	case csEquatorial:
		return new BodyMeanEquatorFrame(obj, obj);
	case csBodyFixed:
		return new BodyFixedFrame(obj, obj);
	case csObjectSync:
		return new ObjectSyncFrame(obj, target);
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
: Frame(obj), fixedObject(tgt)
{
}

quatd_t BodyFixedFrame::getOrientation(double tjd) const
{
	quatd_t q;

	switch (fixedObject->getType()) {
	case objCelestialBody:
		return dynamic_cast<const CelestialBody *>(fixedObject)->getBodyFixed(tjd);
	default:
		return quatd_t(1, 0, 0, 0);
	}
}

// ******** Body Mean Equator Reference Frame ********

BodyMeanEquatorFrame::BodyMeanEquatorFrame(const Object *obj, const Object *tgt)
: Frame(obj), equatorObject(tgt)
{
}

quatd_t BodyMeanEquatorFrame::getOrientation(double tjd) const
{
	quatd_t q;

	switch (equatorObject->getType()) {
	case objCelestialBody:
		return dynamic_cast<const CelestialBody *>(equatorObject)->getEquatorial(tjd);
	default:
		return quatd_t(1, 0, 0, 0);
	}
}

// ******** Object Synchronous Reference Frame ********

ObjectSyncFrame::ObjectSyncFrame(const Object *obj, const Object *tgt)
: Frame(obj), targetObject(tgt)
{
}

quatd_t ObjectSyncFrame::getOrientation(double tjd) const
{
	vec3d_t opos = center->getPosition(tjd);
	vec3d_t tpos = targetObject->getPosition(tjd);

	return glm::lookAt(opos, tpos, vec3d_t(0, 1, 0));
}

// ******** Two Vector Reference Frame ********

FrameVector::FrameVector(FrameVectorType type)
: type(type)
{
}

FrameVector *FrameVector::create(const Object *obs, const Object *tgt,
		FrameVectorType type)
{
	FrameVector *fv = new FrameVector(type);

	fv->obsObject = obs;
	fv->tgtObject = tgt;

	return fv;
}

vec3d_t FrameVector::direction(double tjd) const
{
	vec3d_t v;

	switch (type)
	{
	case fvRelativePosition:
		v = tgtObject->getPosition(tjd) - obsObject->getPosition(tjd);
		break;

	case fvRelativeVelocity:
		v = tgtObject->getVelocity(tjd) - obsObject->getVelocity(tjd);
		break;

	default:
		v = vec3d_t();
		break;
	}

	return v;
}
