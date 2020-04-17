/*
 * timeline.cpp
 *
 *  Created on: Apr 13, 2020
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"
#include "universe/frame.h"
#include "universe/timeline.h"


Phase *Phase::create()
{
	return nullptr;
}

// ********************************************************

bool Timeline::includes(double t) const
{
	return phases.front()->getStartTime() <= t && t < phases.back()->getEndTime();
}

Phase *Timeline::findPhase(double t) const
{
	if (phases.size() == 1)
		return phases[0];

	for (auto phase : phases) {
		if (phase->includes(t))
			return phase;
	}

	return nullptr;
}

bool Timeline::appendPhase(Phase *phase)
{
	if (phases.back()->getEndTime() != phase->getStartTime())
		return false;
	phases.push_back(phase);
	return true;
}
