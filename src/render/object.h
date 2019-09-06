/*
 * object.h - Visual object package
 *
 *  Created on: Aug 22, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "engine/object.h"
#include "render/render.h"

class vObject {
public:
    vObject(const Scene &scene, const Object &object);
    virtual ~vObject() = default;

    virtual void render(renderParameter &prm) = 0;

protected:
    const Scene  &scene;
    const Object &object;
};
