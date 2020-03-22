/*
 * view.h - controllable window view package
 *
 *  Created on: Mar 22, 2020
 *      Author: Tim Stark
 */

#pragma once

class Player;
class Scene;

class View
{
public:
	enum viewType {
		viewMainWindow    = 1,
		viewMultiFunction = 2,
	};

	View() = default;
	~View() = default;

	inline viewType getType() const { return type; }

	inline bool isRoot() const { return parent == nullptr; }

//	void map(float mx, float my, float &pickX, float &pickY);

private:
	viewType type;

protected:
	// Hierarchy view tree
	View *parent = nullptr;
	View *silding = nullptr;
	View *child = nullptr;

	Player *player = nullptr;
	Scene *renderer = nullptr;

	// View screen parameter
	float width, height;
	float x, y;
};
