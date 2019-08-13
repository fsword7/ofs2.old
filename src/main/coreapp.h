/*
 * core.h - OFS core application
 *
 *  Created on: Oct 2, 2018
 *      Author: Tim Stark
 */

#pragma once

//class Engine;
//class Universe;
//class Scene;
//class Player;
class Scene;

#define OFS_KEY_UP		0
#define OFS_KEY_DOWN	1
#define OFS_KEY_REPEAT	2

namespace ofs {
	class CoreApp
	{
	public:
		CoreApp();
		virtual ~CoreApp();

		enum keyCode {
	        keyUndefined = 0,
			keyUp       = 1,
			keyDown     = 2,
			keyLeft     = 3,
			keyRight    = 4,
			keyHome     = 5,
			keyEnd      = 6,
			keyPageUp   = 7,
			keyPageDown = 8,
			keyInsert   = 9,
			keyDelete   = 10,
			keyF0       = 11,
			keyF1       = 12,
			keyF2       = 13,
			keyF3       = 14,
			keyF4       = 15,
			keyF5       = 16,
			keyF6       = 17,
			keyF7       = 18,
			keyF8       = 19,
			keyF9       = 20,
			keyF10      = 21,
			keyF11      = 22,
			keyF12      = 23,

			keyPad0     = 30,
			keyPad1     = 31,
			keyPad2     = 32,
			keyPad3     = 33,
			keyPad4     = 34,
			keyPad5     = 35,
			keyPad6     = 36,
			keyPad7     = 37,
			keyPad8     = 38,
			keyPad9     = 39
		};

		virtual void init() = 0;
		virtual void clean() = 0;
		virtual void run() = 0;

		void initRenderer();
		void initEngine();

		void pressKey(keyCode code, bool down);

		void start();
		void tick();
		void render();
		void resize(int w, int h);

	protected:
		int width, height;

	private:
//		Player   *player;
//		Engine   *engine;
//		Universe *universe;
		Scene *scene = nullptr;

		// keyboard handle parameters
		bool	stateKey[256];
		double	keyRotationAccel;
		double	keyTravelAccel;
		double	keyTravelBrake;
	};
}
