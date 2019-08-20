/*
 * player.h - OFS player package
 *
 *  Created on: Aug 20, 2019
 *      Author: Tim Stark
 */

#pragma once

class Camera
{
public:
    Camera() = default;
    ~Camera() = default;

    inline vec3f_t getPosition() const { return camPosition; };
    inline quatf_t getRotation() const { return camRotation; };
    
private:
    vec3f_t camPosition;
    quatf_t camRotation;
    float   fov; 
}

class Player
{
public:
    Player() = default;
    ~Player() = default;

private:
};
