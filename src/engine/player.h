/*
 * player.h - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

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
	void setFOVrad(double fov);
	void setFOVdeg(double fov);

	void focus(Object *obj);
	void update();
    
private:
    Player  *player;
    vec3d_t  camPosition;
    quatd_t  camRotation;
    double   fov;
    double   aspect;
	double   tanap;
};

class Player
{
public:
    Player();
    ~Player();

	inline vec3d_t getPosition() const   { return lpos; }
	inline quatd_t getRotation() const   { return lqrot; }
	inline double  getJulianTime() const { return jdTime; }

	inline vec3d_t getAngularVelocity() { return av; }
	inline vec3d_t getTravelVelocity()  { return tv; }
	inline double  getTravelSpeed()     { return tv.z; }



    Camera *getCamera(int idx) const;
	
	void setPosition(vec3d_t pos);
	void setAngularVelocity(vec3d_t av);
	void setTravelVelocity(vec3d_t tv);
	void setTravelSpeed(double ts);

    void update(double dt, double timeTravel);

private:
	// Universe position, orientation, and velocity
	vec3d_t	upos, uvec;
	quatd_t	uqrot;

	// Local position, orientation, and velocity in local reference frame
	vec3d_t	lpos, lvec;
	quatd_t	lqrot;

	// Current real time and virtual julian time
	double  realTime;
	double  jdTime;

	// Velocity control
	vec3d_t	av; // Angular velocity
	quatd_t wv; //    Quaternion control
	vec3d_t	tv; // Travel velocity

	vector<Camera *> camera;
};
