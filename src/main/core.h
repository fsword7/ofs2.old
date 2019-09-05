/*
 * core.h - OFS core header package
 *
 *  Created on: Aug 12, 2019
 *      Author: Tim Stark
 */

#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>

#include <fmt/printf.h>

using namespace std;

#include "util/color.h"
#include "main/math.h"

#define APP_FULL_NAME	"Orbital Flight Simulator"
#define APP_SHORT_NAME	"OFS"

// Default window scene size
#define OFS_DEFAULT_WIDTH	1600
#define OFS_DEFAULT_HEIGHT	900
#define OFS_DEFAULT_FOV		50.0f
