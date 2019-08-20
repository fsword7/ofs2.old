/*
 * object.cpp - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#include "main/core.h"
#include "engine/object.h"

Object::Object(ObjectType type)
: objType(type), objNames(1)
{
    objNames.clear();
}

Object::Object(ObjectType type, const string &name)
: objType(type), objNames(1)
{
    objNames[0] = name;
}

Object::~Object()
{
    objNames.clear();
}