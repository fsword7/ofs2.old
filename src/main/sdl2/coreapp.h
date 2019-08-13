/*
 * coreapp.h
 *
 *  Created on: Nov 27, 2018
 *      Author: Tim Stark
 */

#pragma once

#include "main/coreapp.h"

class sdlCoreApp : public ofs::CoreApp
{
public:
	sdlCoreApp();
	~sdlCoreApp();

	void init();
	void clean();
	void run();

	void pressKeyEvent(SDL_KeyboardEvent *key, bool down);

private:
	SDL_Window *dWindow;

};
