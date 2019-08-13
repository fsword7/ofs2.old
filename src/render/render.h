/*
 * render.h
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/gl/context.h"

class Render
{
public:
	Render() = default;
	~Render() = default;

	void init();

private:
	Context gl;
};
