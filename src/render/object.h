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
    vObject(Scene &scene, const Object &object);
    virtual ~vObject() = default;

    static vObject *create(const Object *object, Scene *scene);

    inline const Object *getObject() const { return &object; }

    virtual void render(renderParameter &prm) = 0;

protected:
    Scene  &scene;
    const Object &object;
};
