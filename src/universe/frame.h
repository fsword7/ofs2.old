/*
 * frame.h - Reference frame package
 *
 *  Created on: Oct 17, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "main/math.h"
#include "universe/astro.h"

class Object;

namespace ofs::universe
{
	class CelestialBody;
	class CelestialStar;

	class Frame
	{
	public:
		Frame(const Object *object = nullptr, Frame *parent = nullptr);
		virtual ~Frame() = default;

		inline string getName() const { return frameName; }
		inline const Object *getCenter() const { return center; }
		inline Frame *getParentFrame() const { return parentFrame; }
		inline bool isRoot() const { return parentFrame == nullptr; }

		virtual quatd_t getOrientation(double tjd) const = 0;

		int lock() const;
		int release() const;

		// Universal/Astrocentric conversion calls
		vec3d_t fromAstrocentric(const vec3d_t &upos, double tjd);
		vec3d_t toAstrocentric(const vec3d_t &lpos, double tjd);

		vec3d_t fromUniversal(const vec3d_t &upos, double tjd);
		vec3d_t toUniversal(const vec3d_t &lpos, double tjd);

		quatd_t fromUniversal(const quatd_t &urot, double tjd);
		quatd_t toUniversal(const quatd_t &lrot, double tjd);

		static Frame *create(const string &frameName,
			const Object *bodyObject, const Object *parentObject = nullptr);

	private:
		Frame *parentFrame = nullptr;
		mutable int refCount = 0;

	protected:
		string frameName;
		const Object *center;
	};

	class CachingFrame : public Frame
	{

	};

//	class ReferenceFrameTree
//	{
//	public:
//		ReferenceFrameTree(CelestialStar *star);
//		ReferenceFrameTree(CelestialBody *body);
//		~ReferenceFrameTree();
//
//		inline CelestialStar *getStar() const { return starParent; }
//		inline CelestialBody *getBody() const { return bodyParent; }
//		inline int getSystemSize() const { return objects.size(); }
//
//		inline bool isRoot() const { return bodyParent == nullptr; }
//
//		void addObject(Object *object);
//
//		Object *getObject(int idx) const;
//
//	private:
//		CelestialStar *starParent = nullptr;
//		CelestialBody *bodyParent = nullptr;
//
//		vector<Object *> objects;
//
//		ReferenceFrame *defaultFrame = nullptr;
//	};

	class FrameTree
	{
	public:
		FrameTree(CelestialBody *body);
		FrameTree(CelestialStar *star);
		~FrameTree();

		inline CelestialStar *getStar() const { return starParent; }
		inline CelestialBody *getBody() const { return bodyParent; }
		inline int getSystemSize() const { return objects.size(); }

		bool isRoot() const { return bodyParent == nullptr; }
		inline Frame *getDefaultReferenceFrame() const { return defaultFrame; }

		void addObject(Object *object);

		Object *getObject(int idx) const;

	private:
		CelestialBody *bodyParent = nullptr;
		CelestialStar *starParent = nullptr;

		vector<Object *> objects;

		Frame *defaultFrame;
	};

	class PlayerFrame
	{
	public:
		enum coordType {
			csUniversal  = 0,
			csEcliptical = 1,
			csEquatorial = 2,
			csBodyFixed  = 3,
			csObjectSync = 4
		};

		PlayerFrame();
		PlayerFrame(coordType csType, const Object *center = nullptr,
			const Object *target = nullptr);
		~PlayerFrame();

		string getName() const;

//		vec3d_t fromAstrocentric(const vec3d_t &upos, double tjd);
//		vec3d_t toAstrocenteric(const vec3d_t &lpos, double tjd);
		vec3d_t fromUniversal(const vec3d_t &upos, double tjd);
		vec3d_t toUniversal(const vec3d_t &lpos, double tjd);
		quatd_t fromUniversal(const quatd_t &urot, double tjd);
		quatd_t toUniversal(const quatd_t &lrot, double tjd);

		Frame *create(coordType csType, const Object *center = nullptr,
			const Object *target = nullptr);

		const Object *getCenter() const { return frame != nullptr ? frame->getCenter() : nullptr; }

		coordType getCoordType() { return type; }

	private:
		coordType type = csUniversal;
		Frame *frame   = nullptr;
	};

	class J2000EclipticFrame : public Frame
	{
	public:
		J2000EclipticFrame(const Object *obj, Frame *parent);
		~J2000EclipticFrame() = default;

		quatd_t getOrientation(double) const override
		{
			return quatd_t(1,0,0,0);
		}
	};

	class J2000EquatorFrame : public Frame
	{
	public:
		J2000EquatorFrame(const Object *obj, const Object *tgt, Frame *parent);
		~J2000EquatorFrame() = default;

		quatd_t getOrientation(double) const override
		{
			return quatd_t(vec3d_t(-J2000Obliquity, 0, 0));
		}
	};

	class BodyFixedFrame : public Frame
	{
	public:
		BodyFixedFrame(const Object *obj, const Object *tgt, Frame *parent);
		~BodyFixedFrame() = default;

		quatd_t getOrientation(double tjd) const;

	private:
		const Object *fixedObject = nullptr;
	};

	class BodyMeanEquatorFrame : public Frame
	{
	public:
		BodyMeanEquatorFrame(const Object *obj, const Object *tgt, Frame *parent);
		~BodyMeanEquatorFrame() = default;

		quatd_t getOrientation(double tjd) const;

	private:
		const Object *equatorObject = nullptr;
	};

	class ObjectSyncFrame : public Frame
	{
	public:
		ObjectSyncFrame(const Object *obj, const Object *tgt, Frame *parent);
		~ObjectSyncFrame() = default;

		quatd_t getOrientation(double tjd) const;

	private:
		const Object *targetObject = nullptr;
	};

	class FrameVector
	{
	public:
		enum FrameVectorType {
			fvRelativePosition,
			fvRelativeVelocity
		};

		static FrameVector *create(const Object *obs, const Object *tgt,
			FrameVectorType type);

		vec3d_t direction(double tjd) const;

	private:
		FrameVector(FrameVectorType type);

	private:
		FrameVectorType type;

		const Object *obsObject = nullptr; // Observer object
		const Object *tgtObject = nullptr; // Target object
	};

	class TwoVectorFrame : public Frame
	{

	};

}
