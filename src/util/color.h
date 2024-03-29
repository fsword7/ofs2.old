/*
 * color.h - OFS color package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

class Color
{
public:
	Color() = default;
	Color(float red, float green, float blue, float alpha = 1.0f)
	: red(red), green(green), blue(blue), alpha(alpha) {}
	~Color() = default;

	// Getter function calls
	inline float getRed() const   { return red; }
	inline float getGreen() const { return green; }
	inline float getBlue() const  { return blue; }
	inline float getAlpha() const { return alpha; }

	// Setter function calls
	inline void setRed(float r)   { red = r; }
	inline void setGreen(float g) { green = g; }
	inline void setBlue(float b)  { blue = b; }
	inline void setAlpha(float a) { alpha = a; }

private:
	// RGBA 32-bit values for SDR/HDR purposes
	float red   = 0.0f;
	float green = 0.0f;
	float blue  = 0.0f;
	float alpha = 1.0f;
};
