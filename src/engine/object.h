/*
 * object.h - OFS object package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

enum ObjectType {
    objUnknown = 0,
    objCelestialBody,
    objCelestialStar
};

class Object
{
public:
    Object(ObjectType type);
    Object(ObjectType type, const string &name);
    virtual ~Object();

private:
    ObjectType objType = objUnknown;

protected:
    vector<string> objNames;

};