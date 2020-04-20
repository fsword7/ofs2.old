/*
 * object.h - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "ephem/orbit.h"
#include "ephem/rotation.h"
#include "universe/frame.h"

using namespace ofs::universe;
using namespace ofs::ephem;

#define OBJ_INVISIBLE  0x8000000000000000L

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
	
    inline string getName(uint32_t idx = 0) const { return idx <= objNames.size() ? objNames[idx] : string(); }

    inline void setName(const string &name) { objNames[0] = name; }

    inline void setColor(Color color) { objColor = color; }
    inline void setPosition(vec3d_t pos) { objPosition = pos; }
    inline void setVelocity(vec3d_t vel) { objVelocity = vel; }
    inline void setRadius(float rad) { objRadius = rad; }

    inline void setOrbitFrame(Frame *frame)  { orbitFrame = frame; }
    inline void setObjectFrame(Frame *frame) { objectFrame = frame; }
    inline void setOrbit(Orbit *orbit)                { this->orbit = orbit; }
    inline void setRotation(RotationModel *rot)       { this->rot = rot; }

//    inline bool checkAnyFlags(uint64_t flags) const
//    	{ return objFlags & flags; }
//    inline bool checkAllFlags(uint64_t flags) const
//    	{ return (objFlags & flags) == flags; }

    inline void setFlag(uint64_t flag)   { objFlags |= flag; }
    inline void clearFlag(uint64_t flag) { objFlags &= ~flag; }
    inline void clearAllFlags()          { objFlags = 0; }

    inline uint64_t getFlags() const { return objFlags; }
    inline Color getColor() const { return objColor; }
    inline double getRadius() const { return objRadius; }
    inline double getCullingRadius() const { return objCullingRadius; }
    inline ObjectType getType() const { return objType; }
    inline Frame *getOrbitFrame() const { return orbitFrame; }
    inline Frame *getObjectFrame() const { return objectFrame; }

//    inline SystemTree *getSystemTree() const { return sysTree; }

//    SystemTree *createSystemTree();

    virtual vec3d_t getPosition(double tjd = 0) const;
    virtual vec3d_t getVelocity(double tjd = 0) const;
	virtual quatd_t getRotation(double tjd = 0) const;

protected:
	virtual double computeCullingRadius();

private:
    ObjectType objType = objUnknown;

protected:
    vector<string> objNames;

    Color		objColor;			// Object color (default)
    uint64_t	objFlags;			// Object flags
	double		objRadius = 0.0;		// Object radius
	double		objCullingRadius = objRadius; // Object culling radius
	vec3d_t		objSemiAxes = {0, 0, 0};	// Object semi-axes
	double		objMass = 0.0;		// Object mass
	double		objAlbedo = 1.0;	// Object albedo

	vec3d_t  objPosition;	// Object current position
	vec3d_t  objVelocity;   // Object current velocity
	quatd_t  objRotation;	// Object orientation

//	// Planetary system parameters
//	SystemTree     *sysTree = nullptr;

	// Orbital parameters
	Frame          *orbitFrame = nullptr;
	Orbit          *orbit = nullptr;

	// Orientation parameters
	Frame          *objectFrame = nullptr;
	RotationModel  *rot = nullptr;
};
