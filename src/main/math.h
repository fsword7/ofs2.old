/*
 * math.h - OFS math package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <cmath>

#define PI (3.14159265358979323846)

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

typedef glm::tvec3<float> vec3f_t;
typedef glm::tvec4<float> vec4f_t;
typedef glm::tquat<float> quatf_t;

typedef glm::fmat3 mat3f_t;
typedef glm::fmat4 mat4f_t;

typedef glm::tvec3<double> vec3d_t;
typedef glm::tvec4<double> vec4d_t;
typedef glm::tquat<double> quatd_t;

typedef glm::dmat3 mat3d_t;
typedef glm::dmat4 mat4d_t;


template<class T>
inline T toRadian(T deg) { return deg * (PI / 180.0); }

template<class T>
inline T toDegree(T rad) { return rad * (180.0 / PI); }

template<class T> inline T clamp(T x)
{
	if (x < 0.0)
		return 0.0;
	else if (x > 1.0)
		return 1.0;
	else return x;
}

#define square(val) ((val) * (val))
#define cube(val)   ((val) * (val) * (val))

// Determine area of a circle calculation
template <typename T> inline constexpr T circleArea(T r)
{
	return static_cast<T>(PI) * r * r;
}

// Determine area of a sphere calculation
template <typename T> inline constexpr T sphereArea(T r)
{
	return 4 * static_cast<T>(PI) * r * r;
}

template <class T>
glm::tquat<T> xrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), sin(ang), 0, 0);
}

template <class T>
glm::tquat<T> yrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), 0, sin(ang), 0);
}

template <class T>
glm::tquat<T> zrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), 0, 0, sin(ang));
}
