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

typedef glm::tvec2<float> vec2f_t;
typedef glm::tvec3<float> vec3f_t;
typedef glm::tvec4<float> vec4f_t;
typedef glm::tquat<float> quatf_t;

typedef glm::fmat3 mat3f_t;
typedef glm::fmat4 mat4f_t;

typedef glm::tvec2<double> vec2d_t;
typedef glm::tvec3<double> vec3d_t;
typedef glm::tvec4<double> vec4d_t;
typedef glm::tquat<double> quatd_t;

typedef glm::dmat3 mat3d_t;
typedef glm::dmat4 mat4d_t;


template <typename T> inline constexpr T toRadian(T deg)
{
	return deg * (PI / 180.0);
}

template <typename T> inline constexpr T toDegree(T rad)
{
	return rad * (180.0 / PI);
}

template <typename T> inline constexpr T clamp(T x)
{
	if (x < 0.0)
		return 0.0;
	else if (x > 1.0)
		return 1.0;
	else return x;
}

template <typename T> inline constexpr T square(T x)
{
	return x * x;
}

template <typename T> inline constexpr T cube(T x)
{
	return x * x * x;
}

// Determine area of a circle calculation
template <typename T> inline constexpr T circleArea(T r)
{
	return PI * r * r;
}

// Determine area of a sphere calculation
template <typename T> inline constexpr T sphereArea(T r)
{
	return 4 * PI * r * r;
}

template <typename T> inline glm::tquat<T> xrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), sin(ang), 0, 0);
}

template <typename T> inline glm::tquat<T> yrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), 0, sin(ang), 0);
}

template <typename T> inline glm::tquat<T> zrot(T radians)
{
	T ang = radians * T(0.5); // half angle
	return glm::tquat<T>(cos(ang), 0, 0, sin(ang));
}
