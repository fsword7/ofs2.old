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

	inline vec3f_t getPosition() const { return camPosition; }
	inline quatf_t getRotation() const { return camRotation; }
	inline float getFOV() const { return fov; }
	inline float getTanAp() const { return tanap; }
	void setFOVrad(double fov);
	void setFOVdeg(double fov);

	void focus(Object *obj);
	void update();
    
private:
    Player  *player;
    vec3f_t  camPosition;
    quatf_t  camRotation;
    float    fov;
	float    tanap;
};

class Player
{
public:
    Player();
    ~Player();

	inline vec3f_t getPosition() const   { return lpos; }
	inline quatf_t getRotation() const   { return lqrot; }
	inline float   getJulianTime() const { return jdTime; }

	inline vec3f_t getAngularVelocity() { return av; }
	inline vec3f_t getTravelVelocity()  { return tv; }
	inline float   getTravelSpeed()     { return tv.z; }



    Camera *getCamera(int idx) const;
	
	void setPosition(vec3f_t pos);
	void setAngularVelocity(vec3f_t av);
	void setTravelVelocity(vec3f_t tv);
	void setTravelSpeed(float ts);

    void update(float dt, float timeTravel);

private:
	// Universe position, orientation, and velocity
	vec3f_t	upos, uvec;
	quatf_t	uqrot;

	// Local position, orientation, and velocity in local reference frame
	vec3f_t	lpos, lvec;
	quatf_t	lqrot;

	// Current real time and virtual julian time
	float   realTime;
	float   jdTime;

	// Velocity control
	vec3f_t	av; // Angular velocity
	quatf_t wv; //    Quaternion control
	vec3f_t	tv; // Travel velocity

	vector<Camera *> camera;
};
