/*
 * universe.h - Universe package
 *
 *  Created on: Aug 25, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "universe/starlib.h"

class Universe
{
public:
    Universe() = default;
    ~Universe() = default;

    void init();
    
private:
    ofs::universe::StarCatalogue starlib;
};