/*
 * object.h - Visual object package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "render/render.h"

class vObject {
public:
    vObject(const Scene &scene);
    virtual ~vObject() = default;

protected:
    const Scene  &scene;
    // const Object &object;
};
