/*
 * player.h - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/frame.h"

using namespace ofs::universe;

class Object;
class Player;

class Camera
{
public:
	Camera(Player *player);
	~Camera();

	inline vec3d_t getPosition() const { return camPosition; }
	inline quatd_t getRotation() const { return camRotation; }
	inline double getFOV() const { return fov; }
	inline double getAspect() const { return aspect; }
	inline double getTanAp() const { return tanap; }

	vec3d_t getPickRay(float vx, float vy) const;

	void setViewport(uint32_t width, uint32_t height);
	void setFieldOfView(double fov);

	void focus(Object *obj);
	void update();
    
private:
    Player  *player;
    vec3d_t  camPosition;
    quatd_t  camRotation;

    uint32_t height = 1, width = 1;
    double   fov;
    double   aspect;
	double   tanap;
};

class Player
{
public:
	enum travelMode {
		tvFree
	};

	enum followMode {
		fwGeoSync,
		fwEcliptic,
		fwHelioSync,
		fwObjectSync
	};

	enum goMode {
		goGeoSync,
		goFrontHelioSync,
		goBackHelioSync,
		goObjectSync
	};

    Player();
    ~Player();

	inline vec3d_t getPosition() const         { return upos; }
	inline vec3d_t getLocalPosition() const    { return lpos; }
	inline quatd_t getOrientation() const      { return uqrot; }
	inline quatd_t getLocalOrientation() const { return lqrot; }
	inline double  getRealTime() const         { return realTime; }
	inline double  getJulianTime() const       { return jdTime; }
	inline double  getCurrentTime() const      { return jdTime; }
	inline double  getDeltaTime() const        { return deltaTime; }
	inline Object *getSelectedObject() const   { return selectObject; }

	inline vec3d_t getAngularVelocity() { return av; }
	inline vec3d_t getTravelVelocity()  { return tv; }
	inline double  getTravelSpeed()     { return tv.z; }

	inline const Object *getCenter() const { return frame->getCenter(); }

    Camera *getCamera(int idx) const;
	vec3d_t getRay(float vx, float vy) const;
	
	void setPosition(vec3d_t pos);
	void setOrientation(quatd_t rot);
	void setAngularVelocity(vec3d_t av);
	void setTravelVelocity(vec3d_t tv);
	void setTravelSpeed(double ts);

    void setFrame(PlayerFrame::coordType cs, const Object *obj,
    		const Object *target = nullptr);

    double computeCoarseness(double maxCoarseness);

    void move(const Object &obj, double dist, goMode mode);
    void follow(const Object &obj, followMode mode);
    void look(const Object &obj);

    void rotate(quatd_t rot);
    void dolly(double dist);
    void orbit(quatd_t rot);

    void start(double jd);
    void update(double dt, double timeTravel);
	void updateFrame(PlayerFrame *nfame);
	void updateUniversal();

private:
    // Reference frame
    PlayerFrame *frame = nullptr;

    // Selection object
    Object *selectObject = nullptr;

	// Universe position, orientation, and velocity
	vec3d_t	upos, uvec;
	quatd_t	uqrot;

	// Local position, orientation, and velocity in local reference frame
	vec3d_t	lpos, lvec;
	quatd_t	lqrot;

	// Current real time and virtual julian time
	double  realTime;
	double  jdTime;
	double  deltaTime;

	travelMode mode = tvFree;

	// Velocity control
	vec3d_t	av; // Angular velocity
	quatd_t wv; //    Quaternion control
	vec3d_t	tv; // Travel velocity

	vector<Camera *> camera;
};
