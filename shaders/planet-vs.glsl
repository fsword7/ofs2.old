#version 420

// vertex buffer objects
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 ePosition;
layout (location = 2) in vec3 vNormal;
//layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

uniform mat4 mvp;

uniform vec3 gCamera;
uniform vec3 eCamera;

out vec4 myColor;
out vec2 texCoord;

void main()
{
    vec3 t1 = ePosition - eCamera;
    vec3 e = t1 - ePosition;
    vec3 t2 = ((-eCamera - e) + (ePosition - (t1 - e))) +
        vPosition - gCamera;
    vec3 hdiff = t1 + t2;
    vec3 ldiff = t2 - (hdiff - t1);
    vec3 pos = hdiff + ldiff;

    gl_Position = mvp * vec4(pos, 1.0);
    myColor = vec4(0.7, 0.7, 0.7, 1.0); // vec4(vColor, 1.0);
    texCoord = vTexCoord;
}
