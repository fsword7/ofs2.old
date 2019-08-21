/*
 * player.cpp - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "engine/player.h"

Camera::Camera(Player *_player)
: player(_player),
  camPosition(0, 0, 0),
  camRotation(1, 0, 0, 0)
{
	setFOVdeg(OFS_DEFAULT_FOV);
}

Camera::~Camera()
{
}

void Camera::setFOVrad(double _fov)
{
	fov = _fov;
}

void Camera::setFOVdeg(double _fov)
{
	fov = toRadian(_fov);
}

void Camera::focus(Object *obj)
{
	vec3f_t opos = obj->getPosition();
	vec3f_t up   = vec3f_t(0, 1, 0);
	mat4f_t m    = glm::lookAt(camPosition, opos, up);

	camRotation = glm::conjugate(glm::quat_cast(m));

//	cout << "Camera Parameters: " << endl;
//	cout << fixed << setprecision(10) << endl;
//	cout << "Object:   (" << opos.x << ", " << opos.y << ", " << opos.z << ")" << endl;
//	cout << "Camera:   (" << camPosition.x << ", " << camPosition.y << ", " << camPosition.z << ")" << endl;
//	cout << "Rotation: (" << camRotation.w << ", " << camRotation.x << ", " << camRotation.y << ", " << camRotation.z << ")" << endl;

}

void Camera::update()
{
	camPosition = player->getPosition();
	camRotation = player->getRotation();
}

// *****************************************************

Player::Player()
: upos(0, 0, 0), uvec(0, 0, 0), uqrot(1, 0, 0, 0),
  lpos(0, 0, 0), lvec(0, 0, 0), lqrot(1, 0, 0, 0),
  realTime(0), jdTime(0),
  camera(1)
{
	camera[0] = new Camera(this);

	// Initialize velocity controls
	tv = vec3f_t(0, 0, 0);
	av = vec3f_t(0, 0, 0);
}

Player::~Player()
{
	for (auto &old : camera)
		delete old;
	camera.clear();
}

Camera *Player::getCamera(int idx) const
{
	if (idx < camera.size())
		return camera[idx];
	return nullptr;
}

void Player::setAngularVelocity(vec3f_t _av)
{
	av = _av;
	wv = quatf_t(0, av.x, av.y, av.z);
}

void Player::setTravelVelocity(vec3f_t _tv)
{
	tv = _tv;
}

void Player::setTravelSpeed(float ts)
{
	tv.z = ts;
}

void Player::update(float dt, float timeTravel)
{
	// realTime += dt / SECONDS_PER_DAY;
	// jdTime   += (dt / SECONDS_PER_DAY) * timeTravel;

	// Free travel mode
	// Update current position and orientation (local reference frame)
	// Applying angular velocity to rotation quaternion in local space.
	//
	//      dq/dt = q * w * t/2
	//		w = (0, x, y, z)
	//
	lqrot += lqrot * wv * (dt / 2.0f);
	lpos  -= lqrot * tv * dt;

	for (auto cam : camera)
		cam->update();
}
