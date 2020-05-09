/*
 * rotation.h - Rotation Model Package
 *
 *  Created on: Oct 8, 2019
 *      Author: Tim Stark
 */

#pragma once

namespace ofs::ephem
{
	class RotationalModel
	{
	public:
		virtual ~RotationalModel() = default;

		quatd_t getRotation(double tjd) const;

		virtual quatd_t spin(double tjd) const = 0;
		virtual vec3d_t getAngularVelocity(double tjd) const;
		virtual quatd_t getEquatorRotation(double) const;

		virtual double getPeriod() const  { return 0.0; }
		virtual bool isPeriodic() const   { return false; }

		static RotationalModel *create(const string &name);
	};

	class UniformRotationalModel : public RotationalModel
	{
	public:
		UniformRotationalModel(double epoch, double offset,
			double inclination, double ascendingNode,
			double period)
		: epoch(epoch), offset(offset),
		  inclination(inclination),
		  ascendingNode(ascendingNode),
		  period(period) {};

		quatd_t spin(double tjd) const;
		quatd_t getEquatorOrientation(double tjd) const;
		vec3d_t getAngularVelocity(double tjd) const;

		virtual double  getPeriod() const override { return period; }
		virtual bool    isPeriodic() const override { return true; }

	protected:
		double epoch = 0;
		double offset = 0.0;		// rotation at epoch
		double inclination = 0.0;	// rotational axis tilt
		double ascendingNode = 0.0; // Longitude of ascending node of equator
		double period = 0.0;
	};

	class CachingRotationalModel : public RotationalModel
	{
	public:
		CachingRotationalModel() = default;
		virtual ~CachingRotationalModel() = default;

		quatd_t spin(double tjd) const;
		quatd_t getEquatorRotation(double tjd) const;
		vec3d_t getAngularVelocity(double tjd) const;

		virtual quatd_t computeSpin(double tjd) const = 0;
		virtual quatd_t computeEquatorRotation(double) const = 0;
		virtual vec3d_t computeAngularVelocity(double tjd) const;
		virtual double  getPeriod() const = 0;
		virtual bool    isPeriodic() const = 0;

	private:
		mutable quatd_t lastSpin = {1, 0, 0, 0};
		mutable quatd_t lastEquator = {1, 0, 0, 0};
		mutable vec3d_t lastVelocity = {0, 0, 0};

		mutable double  lastTime = 0;
		mutable bool    validSpin = false;
		mutable bool    validEquator = false;
		mutable bool    validVelocity = false;
	};

	class IAURotationalModel : public CachingRotationalModel
	{
	public:
		IAURotationalModel(double period) : period(period) {};
		~IAURotationalModel() = default;

		virtual quatd_t computeSpin(double tjd) const override;
		virtual quatd_t computeEquatorRotation(double tjd) const override;
//		virtual vec3d_t computeAngularVelocity(double tjd) const override;

		virtual double  getPeriod() const override { return period; }
		virtual bool    isPeriodic() const override { return true; }

		virtual vec2d_t computePole(double tjd) const = 0;
		virtual double  computeMeridian(double tjd) const = 0;

	protected:
		double period = 0.0;
		bool   reversal = false;
	};

	class EarthRotationModel : public CachingRotationalModel
	{
	public:
		EarthRotationModel() = default;

		quatd_t computeSpin(double tjd) const override;
		quatd_t computeEquatorRotation(double tjd) const override;

		double getPeriod() const override { return 23.9344694 / 24.0; }
		bool isPeriodic() const override { return true; }
	};
}
