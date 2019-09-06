/*
 * body.h - Celestial body package
 *
 *  Created on: Aug 26, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "engine/object.h"

namespace ofs::universe {
    class CelestialBody : public Object
    {
    public:
        CelestialBody();
        ~CelestialBody() = default;

    private:
    };
}
