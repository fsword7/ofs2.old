/*
 * player.cpp - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "main/date.h"
#include "universe/frame.h"
#include "engine/object.h"
#include "engine/player.h"

using namespace ofs::universe;

// ******** Camera ********

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
	tanap = tan(fov);
}

void Camera::setFOVdeg(double _fov)
{
	fov = glm::radians(_fov);
	tanap = tan(fov);
}

void Camera::focus(Object *obj)
{
	vec3d_t opos = obj->getPosition();
	vec3d_t up   = vec3d_t(0, 1, 0);
	mat4d_t m    = glm::lookAt(camPosition, opos, up);

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

// ******** Player ********

Player::Player()
: upos(0, 0, 0), uvec(0, 0, 0), uqrot(1, 0, 0, 0),
  lpos(0, 0, 0), lvec(0, 0, 0), lqrot(1, 0, 0, 0),
  realTime(0), jdTime(0),
  camera(1)
{
	camera[0] = new Camera(this);

	// Initialize velocity controls
	tv = vec3d_t(0, 0, 0);
	av = vec3d_t(0, 0, 0);

	// Reference frame - universal as default
	frame = new PlayerFrame();
	updateFrame(frame);
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

void Player::setPosition(vec3d_t pos)
{
	lpos = pos;
	for (auto cam : camera)
		cam->update();
}

void Player::setAngularVelocity(vec3d_t _av)
{
	av = _av;
	wv = quatd_t(0, av.x, av.y, av.z);
}

void Player::setTravelVelocity(vec3d_t _tv)
{
	tv = _tv;
}

void Player::setTravelSpeed(double ts)
{
	tv.z = ts;
}

void Player::update(double dt, double timeTravel)
{	
	realTime += dt / SECONDS_PER_DAY;
	jdTime   += (dt / SECONDS_PER_DAY) * timeTravel;

	// Free travel mode
	// Update current position and orientation (local reference frame)
	// Applying angular velocity to rotation quaternion in local space.
	//
	//      dq/dt = q * w * t/2
	//		w = (0, x, y, z)
	//
	lqrot += lqrot * wv * (dt / 2.0f);
	lpos  -= lqrot * tv * dt;

	// Updating current universal coordinates
	updateUniversal();
	for (auto cam : camera)
		cam->update();
}

void Player::updateUniversal()
{
	upos  = frame->toUniversal(lpos, jdTime);
	uqrot = frame->toUniversal(lqrot, jdTime);
}

void Player::updateFrame(PlayerFrame *nframe)
{
	lpos = nframe->fromUniversal(upos, jdTime);
	lqrot = nframe->fromUniversal(uqrot, jdTime);
}

void Player::setFrame(PlayerFrame::CoordType cs, const Object *obj)
{
	PlayerFrame *nframe = new PlayerFrame(cs, obj);
	if (nframe == nullptr) {
		std::cout << "Something wrong..." << std::endl;
		return;
	}
	if (frame != nullptr)
		delete frame;

	updateFrame(nframe);
	frame = nframe;
}


void Player::follow(const Object &obj)
{
	setFrame(PlayerFrame::csEcliptical, &obj);
	std::cout << "Reference Frame: " << frame->name() << std::endl;
}

void Player::look(const Object &obj)
{
	vec3d_t opos = obj.getPosition(jdTime);
	vec3d_t up   = vec3d_t(0, 1, 0);

	uqrot = glm::lookAt(upos, opos, up);
	lqrot = frame->fromUniversal(uqrot, jdTime);

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "Universal Rotation: (" << uqrot.w() << "," << uqrot.x() << "," << uqrot.y() << "," << uqrot.z() << ")" << std::endl;
//	std::cout << "Local Rotation:     (" << lqrot.w() << "," << lqrot.x() << "," << lqrot.y() << "," << lqrot.z() << ")" << std::endl;
}

void Player::go(const Object &obj)
{
	vec3d_t opos = obj.getPosition(jdTime);

	upos = opos;
	upos.z += obj.getRadius() * 4;
	lpos = frame->fromUniversal(upos, jdTime);

	look(obj);

	// Update all cameras
	for (auto cam : camera)
		cam->update();

//	lvel.x() = -astro::getOrbitalVelocity(obj.getMass(), lpos.norm())  * 0.71;
//	lvel.y() = -astro::getOrbitalVelocity(obj.getMass(), lpos.norm())  * 0.71;

//	std::cout << "Orbital velocity: " << std::fixed << (lvel.norm() * 3600 * 0.62) << std::endl;
//	std::cout << "Radius: " << lpos.norm() << std::endl;

//	orbit = new EllipticalOrbit(lpos, lvel, obj.getMass(), jdTime);

//	std::cout << "Universal: (" << std::fixed << upos.x() << "," << upos.y() << "," << upos.z() << ")" << std::endl;
//	std::cout << "Local:     (" << std::fixed << lpos.x() << "," << lpos.y() << "," << lpos.z() << ")" << std::endl;
}
