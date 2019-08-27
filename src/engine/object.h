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

class Object
{
public:
    Object(ObjectType type);
    Object(ObjectType type, const string &name);
    virtual ~Object();
	
    inline string getName(uint32_t idx = 0) { return idx < objNames.size() ? objNames[idx] : string(); }

    inline void setName(const string &name) { objNames[0] = name; }

    vec3f_t getPosition(float dt = 0) const;
	quatf_t getRotation(float dt = 0) const;

private:
    ObjectType objType = objUnknown;

protected:
    vector<string> objNames;

	double  objRadius;		// Object radius
	vec3f_t objSemiAxes;	// Object semi-axes
	double  objMass;		// Object mass
	double  objAlbedo;		// Object albedo

	vec3f_t objPosition;	// Object current position
	quatf_t objRotation;	// Object orientation
};