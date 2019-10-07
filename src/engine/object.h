/*
 * object.h - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

enum ObjectType {
    objUnknown = 0,
    objCelestialBody,
    objCelestialStar
};

class Orbit;

class Object
{
public:
    Object(ObjectType type);
    Object(ObjectType type, const string &name);
    virtual ~Object();
	
    inline string getName(uint32_t idx = 0) const { return idx <= objNames.size() ? objNames[idx] : string(); }

    inline void setName(const string &name) { objNames[0] = name; }

    inline void setPosition(vec3d_t pos) { objPosition = pos; }
    inline void setVelocity(vec3d_t vel) { objVelocity = vel; }
    inline void setRadius(float rad) { objRadius = rad; }

    inline double getRadius() const { return objRadius; }
    inline ObjectType getType() const { return objType; }

    vec3d_t getPosition(double tjd = 0) const;
    vec3d_t getVelocity(double tjd = 0) const;
	quatd_t getRotation(double tjd = 0) const;

private:
    ObjectType objType = objUnknown;

protected:
    vector<string> objNames;

	double  objRadius;		// Object radius
	vec3d_t objSemiAxes;	// Object semi-axes
	double  objMass;		// Object mass
	double  objAlbedo;		// Object albedo

	vec3d_t objPosition;	// Object current position
	vec3d_t objVelocity;    // Object current velocity
	quatd_t objRotation;	// Object orientation

	Orbit *orbit = nullptr;
};
