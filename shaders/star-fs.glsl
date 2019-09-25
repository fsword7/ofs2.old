#version 420

in vec4  starColor;
out vec4 fragColor;

//layout (binding = 0) uniform sampler2D starTex;

void main()
{
    fragColor = /* texture(starTex, gl_PointCoord) * */ starColor;
}
