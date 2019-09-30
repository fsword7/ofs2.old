/*
 * main.cpp - OFS SDL2-based main package
 *
 *  Created on: Aug 13, 2019
 *      Author: Tim Stark
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "main/core.h"
#include "render/gl/context.h"
#include "main/sdl2/coreapp.h"

using namespace ofs;

// Initialize SDL2 facility with OpenGL
void sdlCoreApp::init()
{
	// SDL initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "OFS: Unable to initialize SDL: " << SDL_GetError() << std::endl;
		abort();
	}

	// TTF initialization
	if (TTF_Init() != 0) {
		std::cerr << "OFS: Unable to initialize TTF: " << TTF_GetError() << std::endl;
		abort();
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// OpenGL window/full screen
	dWindow = SDL_CreateWindow(APP_FULL_NAME,
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			OFS_DEFAULT_WIDTH, OFS_DEFAULT_HEIGHT,
			SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	if (dWindow == nullptr) {
		cerr << "SDL2 Window can't be created: " << SDL_GetError() << endl;
		exit(1);
	}
	auto ctx = SDL_GL_CreateContext(dWindow);


	GLenum err = glewInit();
	if (err != GLEW_OK) {
		cerr << "GLEW Error: " << glewGetErrorString(err) << endl;
		exit(1);
	} else {
		cout << "Using GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
		cout << "  OpenGL version:   " << glGetString(GL_VERSION) << endl;
	}
}

void sdlCoreApp::clean()
{
	TTF_Quit();
	SDL_Quit();
}

void sdlCoreApp::run()
{
	bool running = true;
	int  w, h;

	start();
	while (running) {
		SDL_Event event;

		while(SDL_PollEvent(&event) != 0) {
			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;

			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
					SDL_GetWindowSize(dWindow, &w, &h);
					resize(w, h);
				}
				break;

			case SDL_KEYDOWN:
				pressKeyEvent(&event.key, true);
				break;
			case SDL_KEYUP:
				pressKeyEvent(&event.key, false);
				break;
			}
		}

		tick();
		render();
		SDL_GL_SwapWindow(dWindow);
	}
}

void sdlCoreApp::pressKeyEvent(SDL_KeyboardEvent *key, bool down)
{
	CoreApp::keyCode code = keyUndefined;

	switch(key->keysym.sym)
	{
	case SDLK_LEFT:  code = keyLeft;  break;
	case SDLK_RIGHT: code = keyRight; break;
	case SDLK_UP:    code = keyUp;    break;
	case SDLK_DOWN:  code = keyDown;  break;
	case SDLK_KP_0:  code = keyPad0;  break;
	case SDLK_KP_1:  code = keyPad1;  break;
	case SDLK_KP_2:  code = keyPad2;  break;
	case SDLK_KP_3:  code = keyPad3;  break;
	case SDLK_KP_4:  code = keyPad4;  break;
	case SDLK_KP_5:  code = keyPad5;  break;
	case SDLK_KP_6:  code = keyPad6;  break;
	case SDLK_KP_7:  code = keyPad7;  break;
	case SDLK_KP_8:  code = keyPad8;  break;
	case SDLK_KP_9:  code = keyPad9;  break;
    case SDLK_F1:    code = keyF1;    break;
    case SDLK_F2:    code = keyF2;    break;
	}

	if (code > 0)
		pressKey(code, down);
}

int main(int argc, char **argv)
{
	CoreApp &app = *new sdlCoreApp();

	std::cout << "Orbital Flight Simulator" << std::endl;

	app.init();
	app.initEngine();
//	app.initRenderer();

	app.run();

	app.clean();

	return 0;
}




