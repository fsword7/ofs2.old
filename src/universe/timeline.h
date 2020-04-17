/*
 * timeline.h - Object timeline package
 *
 *  Created on: Apr 13, 2020
 *      Author: Tim Stark
 */

#pragma once

class Object;

class Phase
{
public:
	Phase() = default;
	virtual ~Phase() = default;

	inline Object *getObject() const { return object; }

	inline bool includes(double t) const { return startTime <= t && t < endTime; }
	inline double getStartTime() const { return startTime; }
	inline double getEndTime() const { return endTime; }

	static Phase *create();

protected:
	Object *object = nullptr;

	double startTime = -numeric_limits<double>::infinity();
	double endTime = numeric_limits<double>::infinity();
};

class Timeline
{
public:
	Timeline() = default;
	virtual ~Timeline() = default;

	bool appendPhase(Phase *phase);

	Phase *findPhase(double t) const;

	bool includes(double t) const;

	inline int count() const { return phases.size(); }

protected:
	vector<Phase *> phases;
};
