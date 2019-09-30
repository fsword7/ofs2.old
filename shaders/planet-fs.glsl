#version 420
// #extension GL_ARB_shading_language_420pack: enable  Use for GLSL versions before 420.

layout (binding = 0) uniform sampler2D sTexture;

in vec2 texCoord;
in vec4 myColor;

out vec4 fragColor;

void main()
{
    fragColor = texture(sTexture, texCoord);
    // fragColor = myColor;
}
