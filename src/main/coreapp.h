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
			keyPad9     = 39,

			keyA		= 'A',
			keyB		= 'B',
			keyC		= 'C',
			keyD		= 'D',
			keyE		= 'E',
			keyF		= 'F',
			keyG		= 'G',
			keyH		= 'H',
			keyI		= 'I',
			keyJ		= 'J',
			keyK		= 'K',
			keyL		= 'L',
			keyM		= 'M',
			keyN		= 'N',
			keyO		= 'O',
			keyP		= 'P',
			keyQ		= 'Q',
			keyR		= 'R',
			keyS		= 'S',
			keyT		= 'T',
			keyU		= 'U',
			keyV		= 'V',
			keyW		= 'W',
			keyX		= 'X',
			keyY		= 'Y',
			keyZ		= 'Z',

			keya		= 'a',
			keyb		= 'b',
			keyc		= 'c',
			keyd		= 'd',
			keye		= 'e',
			keyf		= 'f',
			keyg		= 'g',
			keyh		= 'h',
			keyi		= 'i',
			keyj		= 'j',
			keyk		= 'k',
			keyl		= 'l',
			keym		= 'm',
			keyn		= 'n',
			keyo		= 'o',
			keyp		= 'p',
			keyq		= 'q',
			keyr		= 'r',
			keys		= 's',
			keyt		= 't',
			keyu		= 'u',
			keyv		= 'v',
			keyw		= 'w',
			keyx		= 'x',
			keyy		= 'y',
			keyz		= 'z',

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
		bool	stateKey[256];
		double	keyRotationAccel;
		double	keyRotationBrake;
		double	keyTravelAccel;
		double	keyTravelBrake;

		// mouse handle parameters
		float lastX, lastY;
	};
}
