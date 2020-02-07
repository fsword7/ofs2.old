/*
 * core.h - OFS core application
 *
 *  Created on: Oct 2, 2018
 *      Author: Tim Stark
 */

#pragma once

#include "universe/universe.h"

class Engine;
class Player;
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

		enum mouseButton {
			mouseLeftButton   = 0x01,
			mouseMiddleButton = 0x02,
			mouseRightButton  = 0x04,
			mouseShiftKey     = 0x08,
			mouseControlKey   = 0x10
		};

		enum keyCode {
	        keyUndefined	= 0x000,

			// 0x01-0xFF reserved for ASCII codes

			keyF0		= 0x100,
			keyF1		= 0x101,
			keyF2		= 0x102,
			keyF3		= 0x103,
			keyF4		= 0x104,
			keyF5		= 0x105,
			keyF6		= 0x106,
			keyF7		= 0x107,
			keyF8		= 0x108,
			keyF9		= 0x109,
			keyF10		= 0x10A,
			keyF11		= 0x10B,
			keyF12		= 0x10C,

			keyPad0		= 0x120,
			keyPad1		= 0x121,
			keyPad2		= 0x122,
			keyPad3		= 0x123,
			keyPad4		= 0x124,
			keyPad5		= 0x125,
			keyPad6		= 0x126,
			keyPad7		= 0x127,
			keyPad8		= 0x128,
			keyPad9		= 0x129,

			keyUp		= 0x130,
			keyDown		= 0x131,
			keyLeft		= 0x132,
			keyRight	= 0x133,
			keyHome		= 0x134,
			keyEnd		= 0x135,
			keyPageUp	= 0x136,
			keyPageDown	= 0x137,
			keyInsert	= 0x138,
			keyDelete	= 0x139
		};

		virtual void init() = 0;
		virtual void clean() = 0;
		virtual void run() = 0;

		void initRenderer();
		void initEngine();

		// Keyboard controls
		void keyPress(keyCode code, int modifiers, bool down);
//		void keyDown(keyCode code, int modifiers);
//		void keyUp(keyCode code, int modifiers);
		void keyEntered(wchar_t ch, int modifiers);

		// Mouse controls
		void mouseMove(float x, float y, int state);
		void mousePressButtonDown(float x, float y, int button);
		void mousePressButtonUp(float x, float y, int button);
		void mouseDialWheel(float motion);

		// Joystick controls
		// GameController controls

		void start();
		void tick();
		void render();
		void resize(int w, int h);

	protected:
		int width, height;

	private:
		Player   *player;
		Engine   *engine;
		Universe *universe;
		Scene *scene = nullptr;

		// keyboard handle parameters
		bool	stateKey[512];
		double	keyRotationAccel;
		double	keyRotationBrake;
		double	keyTravelAccel;
		double	keyTravelBrake;

		// mouse handle parameters
		float lastX, lastY;
	};
}
