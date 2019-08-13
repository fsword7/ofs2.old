/*
 * coreapp.cpp
 *
 *  Created on: Nov 27, 2018
 *      Author: Tim Stark
 */

#include <SDL2/SDL.h>

#include "main/core.h"
#include "render/gl/context.h"
#include "main/sdl2/coreapp.h"

using namespace ofs;

sdlCoreApp::sdlCoreApp()
: CoreApp(),
  dWindow(nullptr)
{
}

sdlCoreApp::~sdlCoreApp()
{
}
