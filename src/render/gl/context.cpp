/*
 * context.cpp
 *
 *  Created on: Aug 13, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "render/gl/context.h"

void Context::init(int w, int h)
{
	resize(w, h);
}

void Context::resize(int w, int h)
{
	width  = w;
	height = h;
	aspect = float(width)/float(height);

	glViewport(0, 0, w, h);

	cout << "Width: " << w << " Height: " << h << " Aspect: " << aspect << endl;
}

void Context::start()
{

}

void Context::finish()
{

}
