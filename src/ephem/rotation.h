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

//		quatd_t getRotation(double tjd) const;

		virtual quatd_t spin(double tjd) const = 0;
		virtual vec3d_t getAngularVelocity(double tjd) const;
		virtual quatd_t getEquatorRotation(double) const;

		virtual double getPeroid() const  { return 0.0; }
		virtual bool isPeroidic() const   { return false; }
	};
}
