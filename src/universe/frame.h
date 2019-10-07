/*
 * frame.h - reference frame package
 *
 *  Created on: Oct 7, 2019
 *      Author: Tim Stark
 */

#pragma once

class Object;

namespace ofs::universe
{
//	class Object;
	class CelestialStar;
	class CelestialBody;

	class SystemTree
	{
	public:
		SystemTree(Object *object);
		~SystemTree() = default;

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
	};

	class ReferenceFrame
	{
	public:
		ReferenceFrame(const Object *object = nullptr);
		virtual ~ReferenceFrame() = default;

		inline const Object *getCenter() const { return center; }

		// Handling reference count
		int lock() const;
		int release() const;

//		vec3d_t fromUniversal(const vec3d_t& upos, double tjd);
//		vec3d_t toUniversal(const vec3d_t& lpos, double tjd);
//		quatd_t fromUniversal(const quatd_t& urot, double tjd);
//		quatd_t toUniversal(const quatd_t& lrot, double tjd);

		virtual quatd_t getRotation(double tjd) = 0;

	private:
		mutable int refCount = 0;

	protected:
		const Object *center = nullptr;
	};

//class EclipticFrame : public ReferenceFrame
//{
//public:
//	EclipticFrame(const Object *obj);
//	~EclipticFrame();
//
//	quatd_t getRotation(double tjd) { return quatd_t::Identity(); }
//};
//
//class EquatorFrame : public ReferenceFrame
//{
//public:
//	EquatorFrame(const Object *obj, const Object *tgt);
//	~EquatorFrame();
//
//	quatd_t getRotation(double tjd) { return quatd_t::Identity(); }
//};
//
//class BodyMeanFrame : public ReferenceFrame
//{
//public:
//	BodyMeanFrame(const Object *obj, const Object *tgt);
//	~BodyMeanFrame();
//
//	quatd_t getRotation(double tjd) { return quatd_t::Identity(); }
//};

}
