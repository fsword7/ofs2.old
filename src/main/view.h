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

	View(viewType type, Player *player, Scene *renderer,
			float wx, float wy, float width, float height)
	: type(type), player(player), renderer(renderer),
	  width(width), height(height), x(wx), y(wy)
	{ }

	~View() = default;

	inline viewType getType() const { return type; }

	inline bool isRootView() const { return parent == nullptr; }
	inline Scene *getRenderer() const { return renderer; }
	inline Player *getPlayer() const { return player; }

	void map(float wx, float wy, float &vx, float &vy) const;

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
	float width = 1.0f, height = 1.0f;
	float x = 0.0f, y = 0.0f;
};
