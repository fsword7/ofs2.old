/*
 * player.cpp - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "main/date.h"
#include "universe/body.h"
#include "universe/star.h"
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
	setFieldOfView(glm::radians(OFS_DEFAULT_FOV));
}

Camera::~Camera()
{
}

void Camera::setViewport(uint32_t width, uint32_t height)
{
	this->width  = width;
	this->height = height;
}

void Camera::setFieldOfView(double fov)
{
	this->fov = fov;
	tanap = tan(this->fov);
}

void Camera::focus(Object *obj)
{
	vec3d_t opos = obj->getPosition();
	vec3d_t up   = vec3d_t(0, 1, 0);
	mat4d_t m    = glm::lookAt(opos, camPosition, up);

	camRotation = glm::conjugate(glm::quat_cast(m));

//	cout << "Camera Parameters: " << endl;
//	cout << fixed << setprecision(10) << endl;
//	cout << "Object:   (" << opos.x << ", " << opos.y << ", " << opos.z << ")" << endl;
//	cout << "Camera:   (" << camPosition.x << ", " << camPosition.y << ", " << camPosition.z << ")" << endl;
//	cout << "Rotation: (" << camRotation.w << ", " << camRotation.x << ", " << camRotation.y << ", " << camRotation.z << ")" << endl;

}

vec3d_t Camera::getPickRay(float vx, float vy) const
{
	float s = float(2.0 * tan(fov/2.0));

	return glm::normalize(vec3d_t(vx * s * aspect, vy * s, -1.0));
}

void Camera::update()
{
	camPosition = player->getPosition();
	camRotation = player->getOrientation();
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

vec3d_t Player::getRay(float vx, float vy) const
{
	if (camera.size() > 0)
		return camera[0]->getPickRay(vx, vy);
	return vec3d_t(0, 0, 0);
}

void Player::setPosition(vec3d_t pos)
{
	upos = pos;
	lpos = frame->fromUniversal(pos, jdTime);
	for (auto cam : camera)
		cam->update();
}

void Player::setOrientation(quatd_t rot)
{
	uqrot = rot;
	lqrot = frame->fromUniversal(rot, jdTime);
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

void Player::start(double jd)
{
	realTime = jd;
	jdTime   = jd;
}

void Player::update(double dt, double timeTravel)
{	
	realTime += dt / SECONDS_PER_DAY;
	jdTime   += (dt / SECONDS_PER_DAY) * timeTravel;
	deltaTime = dt;

	if (mode == tvFree) {
		// Free travel mode
		// Update current position and orientation (local reference frame)
		// Applying angular velocity to rotation quaternion in local space.
		//
		//      dq/dt = q * w * t/2
		//		w = (0, x, y, z)
		//
		lqrot += lqrot * wv * (dt / 2.0);
		lqrot  = glm::normalize(lqrot);
		lpos  -= lqrot * tv * dt;
	}

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

void Player::setFrame(PlayerFrame::coordType cs, const Object *obj, const Object *target)
{
	PlayerFrame *nframe = new PlayerFrame(cs, obj, target);
	if (nframe == nullptr) {
		std::cout << "Something wrong..." << std::endl;
		return;
	}
	if (frame != nullptr)
		delete frame;

	updateFrame(nframe);
	frame  = nframe;
}


void Player::follow(const Object &obj, followMode mode)
{
	PlanetarySystem *system;
	CelestialStar *sun;

	switch (mode) {
	case fwGeoSync:
		setFrame(PlayerFrame::csBodyFixed, &obj);
		break;
	case fwHelioSync:
		if (obj.getType() == objCelestialBody) {
			system = dynamic_cast<const CelestialBody *>(&obj)->getInSystem();
			if (system != nullptr)
				sun = system->getStar();
		}
		setFrame(PlayerFrame::csObjectSync, &obj, sun);
		break;
	case fwEcliptic:
	default:
		setFrame(PlayerFrame::csEcliptical, &obj);
		break;
	}

//	std::cout << "Reference Frame: " << frame->name() << std::endl;
}

void Player::look(const Object &obj)
{
	vec3d_t opos = obj.getPosition(jdTime);
	vec3d_t up   = vec3d_t(0, 1, 0);

	uqrot = glm::lookAt(opos, upos, up);
	lqrot = frame->fromUniversal(uqrot, jdTime);

	// Update all cameras
//	updateUniversal();
	for (auto cam : camera)
		cam->update();

//	std::cout << std::fixed << std::setprecision(10);
//	std::cout << "Universal Rotation: (" << uqrot.w() << "," << uqrot.x() << "," << uqrot.y() << "," << uqrot.z() << ")" << std::endl;
//	std::cout << "Local Rotation:     (" << lqrot.w() << "," << lqrot.x() << "," << lqrot.y() << "," << lqrot.z() << ")" << std::endl;
}

void Player::move(const Object &obj, double dist, goMode mode)
{
	vec3d_t opos = obj.getPosition(jdTime);
	quatd_t orot;

	PlanetarySystem *system;
	CelestialStar *sun;

	switch (mode) {
	case goGeoSync:
		orot = obj.getRotation(jdTime);
		break;
	case goHelioSync:
		if (obj.getType() == objCelestialBody) {
			system = dynamic_cast<const CelestialBody *>(&obj)->getInSystem();
			if (system != nullptr)
				sun = system->getStar();
		}
		vec3d_t tpos = sun->getPosition(jdTime);
		orot = glm::lookAt(opos, tpos, vec3d_t(0, 1, 0));
		orot *= yrot(glm::radians(180.0));
		break;
	}

	upos  = opos + glm::conjugate(orot) * vec3d_t(0, 0, -dist);
	uqrot = glm::conjugate(orot) * quatd_t(1, 0, 0, 0);;
	lpos  = frame->fromUniversal(upos, jdTime);
	lqrot = frame->fromUniversal(uqrot, jdTime);

//	look(obj);

	// Update all cameras
//	updateUniversal();
	for (auto cam : camera)
		cam->update();

//	lvel.x() = -astro::getOrbitalVelocity(obj.getMass(), lpos.norm())  * 0.71;
//	lvel.y() = -astro::getOrbitalVelocity(obj.getMass(), lpos.norm())  * 0.71;

//	std::cout << "Orbital velocity: " << std::fixed << (lvel.norm() * 3600 * 0.62) << std::endl;
//	std::cout << "Radius: " << lpos.norm() << std::endl;

//	orbit = new EllipticalOrbit(lpos, lvel, obj.getMass(), jdTime);

//	cout << "Universal: (" << std::fixed << upos.x << "," << upos.y << "," << upos.z << ")" << endl;
//	cout << "Local:     (" << std::fixed << lpos.x << "," << lpos.y << "," << lpos.z << ")" << endl;
}

double Player::computeCoarseness(double maxCoarseness)
{
	const Object *center = frame->getCenter();
	if (center == nullptr)
		return maxCoarseness;

	double radius   = center->getRadius();
	double distance = glm::length(lpos);
	double altitude = distance - radius;
	double coarse   = maxCoarseness;

	if (altitude > 0 && altitude < radius)
		coarse *= max(0.01, altitude/radius);
	return coarse;
}

void Player::rotate(quatd_t rot)
{
	lqrot = lqrot * rot;

	// Updating current universal coordinates
	updateUniversal();
	for (auto cam : camera)
		cam->update();
}

// Move toward or from the center object at exponential rate.
void Player::dolly(double delta)
{
	const Object *center = frame->getCenter();
	if (center == nullptr)
		return;

	vec3d_t opos  = center->getPosition(jdTime);
	double  surfaceDistance = center->getRadius();
	double  naturalDistance = surfaceDistance * 4.0;
	double  currentDistance = glm::length(lpos);

	if (currentDistance >= surfaceDistance && naturalDistance != 0) {
		double r = (currentDistance - surfaceDistance) / naturalDistance;
		double newDistance = surfaceDistance + naturalDistance * exp(log(r) + delta);

//		cout << fmt::sprintf("Zoom: S: %lf N: %lf C: %lf (%lf,%lf,%lf) * %lf\n",
//			surfaceDistance, naturalDistance, currentDistance,
//			lpos.x, lpos.y, lpos.z, newDistance / currentDistance);
		lpos *= (newDistance / currentDistance);

		// Updating current universal coordinates
		updateUniversal();
		for (auto cam : camera)
			cam->update();
	}

}

void Player::orbit(quatd_t rot)
{
	const Object *center = frame->getCenter();
	if (center == nullptr)
		return;

	double  dist = glm::length(lpos);
	quatd_t qrot = glm::normalize(lqrot * rot * glm::conjugate(lqrot));

	lpos  = glm::normalize(glm::conjugate(qrot) * lpos) * dist;
	lqrot = glm::conjugate(qrot) * lqrot;

	// Updating current universal coordinates
	updateUniversal();
	for (auto cam : camera)
		cam->update();
}
