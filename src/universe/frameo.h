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

	class ReferenceFrame
	{
	public:
		enum FrameType {
			UnknownFrame     = 0,
			PositionFrame    = 1,
			OrientationFrame = 2
		};

		ReferenceFrame(const Object *object = nullptr);
		virtual ~ReferenceFrame() = default;

		inline const Object *getCenter() const { return center; }

		// Handling reference count
		int lock() const;
		int release() const;

		// Universal/Astrocentric/Orientation coordination conversions
		vec3d_t fromUniversal(const vec3d_t& upos, double tjd);
		vec3d_t toUniversal(const vec3d_t& lpos, double tjd);

		quatd_t fromUniversal(const quatd_t& urot, double tjd);
		quatd_t toUniversal(const quatd_t& lrot, double tjd);

		vec3d_t fromAstrocentric(const vec3d_t& upos, double tjd);
		vec3d_t toAstrocentric(const vec3d_t& lpos, double tjd);

		virtual quatd_t getRotation(double tjd) const = 0;
//		virtual vec3d_t getAngularVelocity(double tdb) const = 0;
//		virtual bool isInertial() const = 0;

	private:
		mutable int refCount = 0;

	protected:
		const Object *center = nullptr;
	};

	class PlayerFrame
	{

	public:
		enum CoordType {
			csUniversal  = 0,
			csEcliptical = 1,
			csEquatorial = 2
		};

		PlayerFrame();
		PlayerFrame(CoordType cs, const Object *obj = nullptr);
		~PlayerFrame();

		vec3d_t fromUniversal(const vec3d_t& pos, double tjd);
		quatd_t fromUniversal(const quatd_t& rot, double tjd);
		vec3d_t toUniversal(const vec3d_t& pos, double tjd);
		quatd_t toUniversal(const quatd_t& rot, double tjd);

		ReferenceFrame *create(CoordType cs, const Object *obj = nullptr);

		std::string name() const;

	private:
		CoordType       csType;
		ReferenceFrame *frame;
	};

	class J2000EclipticFrame : public ReferenceFrame
	{
	public:
		J2000EclipticFrame(const Object *obj);
		~J2000EclipticFrame() = default;

		quatd_t getRotation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class J2000EquatorFrame : public ReferenceFrame
	{
	public:
		J2000EquatorFrame(const Object *obj, const Object *tgt);
		~J2000EquatorFrame() = default;

		quatd_t getRotation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class BodyFixedFrame : public ReferenceFrame
	{
	public:
		BodyFixedFrame(const Object *obj, const Object *tgt);
		~BodyFixedFrame() = default;

		quatd_t getRotation(double tjd) const { return quatd_t(1,0,0,0); }
	};

	class BodyMeanEquatorFrame : public ReferenceFrame
	{
	public:
		BodyMeanEquatorFrame(const Object *obj, const Object *tgt);
		~BodyMeanEquatorFrame() = default;

		quatd_t getRotation(double tjd) const;

	private:
		const Object *equatorObject = nullptr;
	};

}
