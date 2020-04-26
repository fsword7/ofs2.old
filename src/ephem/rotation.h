/*
 * rotation.h - Rotation Model Package
 *
 *  Created on: Oct 8, 2019
 *      Author: Tim Stark
 */

#pragma once

namespace ofs::ephem
{
	class RotationModel
	{
	public:
		virtual ~RotationModel() = default;

		quatd_t getRotation(double tjd) const;

		virtual quatd_t spin(double tjd) const = 0;
		virtual vec3d_t getAngularVelocity(double tjd) const;
		virtual quatd_t getEquatorRotation(double) const;

		virtual double getPeriod() const  { return 0.0; }
		virtual bool isPeriodic() const   { return false; }
	};

	class CachingRotationModel : public RotationModel
	{
	public:
		CachingRotationModel() = default;
		virtual ~CachingRotationModel() = default;

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

	class IAURotationModel : public CachingRotationModel
	{
	public:
		IAURotationModel(double poleRA, double pileRARate,
						 double poleDec, double poleDecRate,
						 double meridian, double period);
		~IAURotationModel() = default;

		virtual quatd_t computeSpin(double tjd) const override;
		virtual quatd_t computeEquatorRotation(double tjd) const override;
//		virtual vec3d_t computeAngularVelocity(double tjd) const override;

		virtual double  getPeriod() const override { return period; }
		virtual bool    isPeriodic() const override { return true; }

		vec2d_t computePole(double tjd) const;
		double  computeMeridian(double tjd) const;

	private:
		double poleRA, poleRARate;
		double poleDec, poleDecRate;
		double meridianAtEpoch;
		double rotationRate;
		double period = 0.0;
		bool   reversal = false;
	};

	class EarthRotationModel : public RotationModel
	{
	public:
		EarthRotationModel() = default;

		quatd_t spin(double tjd) const override;
		quatd_t getEquatorRotation(double tjd) const override;

		double getPeriod() const override { return 23.9344694 / 24.0; }
		bool isPeriodic() const override { return true; }
	};
}
