#version 420

// vertex buffer objects
layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
//layout (location=2) in vec3 vColor;
layout (location=2) in vec2 vTexCoord;

uniform mat4 gModelView;
uniform mat4 gProj;

out vec4 myColor;
out vec2 texCoord;

void main()
{
    gl_Position = gProj * gModelView * vec4(vPosition, 1.0);
    myColor = vec4(0.7, 0.7, 0.7, 1.0); // vec4(vColor, 1.0);
    texCoord = vTexCoord;
}
