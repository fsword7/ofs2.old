#version 420

layout (location = 0) in vec3  vPosition;
layout (location = 1) in vec3  vColor;
layout (location = 2) in float vSize;

uniform mat4 mvp;

out vec4 starColor;

void main()
{
    gl_Position  = mvp * vPosition;
    gl_PointSize = vSize;
    starColor = vec4(vColor, 1.0f);
}
