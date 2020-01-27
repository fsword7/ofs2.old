/*
 * frame.h - Reference frame package
 *
 *  Created on: Oct 17, 2019
 *      Author: Tim Stark
 */

#pragma once

class Object;

namespace ofs::universe
{
	class CelestialBody;
	class CelestialStar;

	class Frame
	{
	public:
		Frame(const Object *object = nullptr);
		virtual ~Frame() = default;

		inline const Object *getCenter() const { return center; }

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


	private:
		mutable int refCount = 0;

	protected:
		const Object *center;
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
			csEquatorial = 2
		};

		PlayerFrame();
		PlayerFrame(coordType csType, const Object *center = nullptr);
		~PlayerFrame();

		string getName() const;

//		vec3d_t fromAstrocentric(const vec3d_t &upos, double tjd);
//		vec3d_t toAstrocenteric(const vec3d_t &lpos, double tjd);
		vec3d_t fromUniversal(const vec3d_t &upos, double tjd);
		vec3d_t toUniversal(const vec3d_t &lpos, double tjd);
		quatd_t fromUniversal(const quatd_t &urot, double tjd);
		quatd_t toUniversal(const quatd_t &lrot, double tjd);

		Frame *create(coordType csType, const Object *center = nullptr);

		const Object *getCenter() const { return frame != nullptr ? frame->getCenter() : nullptr; }

		coordType getCoordType() { return type; }

	private:
		coordType type = csUniversal;
		Frame *frame   = nullptr;
	};

	class J2000EclipticFrame : public Frame
	{
	public:
		J2000EclipticFrame(const Object *obj);
		~J2000EclipticFrame() = default;

		quatd_t getOrientation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class J2000EquatorFrame : public Frame
	{
	public:
		J2000EquatorFrame(const Object *obj, const Object *tgt);
		~J2000EquatorFrame() = default;

		quatd_t getOrientation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class BodyFixedFrame : public Frame
	{
	public:
		BodyFixedFrame(const Object *obj, const Object *tgt);
		~BodyFixedFrame() = default;

		quatd_t getOrientation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class BodyMeanEquatorFrame : public Frame
	{
	public:
		BodyMeanEquatorFrame(const Object *obj, const Object *tgt);
		~BodyMeanEquatorFrame() = default;

		quatd_t getOrientation(double tjd) const;

	private:
		const Object *equatorObject = nullptr;
	};

}
